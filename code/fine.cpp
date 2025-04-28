#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <thread>
#include <cmath>
#include <mutex>
#include <atomic>
#include <barrier>
#include "constants.hpp"
#include "helpers.hpp"

void fineWorker(
    int num_threads, int tid,
    const int array_size, const int levels_saved_arg,
    const std::vector<std::array<int, 3>>& ops,
    std::vector<int>& ST,
    std::vector<std::array<int,2>>& query_results,
    std::vector<std::mutex>& ST_mutexes,
    std::barrier<> &batch_barrier,
    const std::vector<int>& batch_starts,
    IntCombine combine_fn)
{
    int queries_completed = 0;
    int batch_iter = 0;
    int num_batches = batch_starts.size();
    int num_levels = std::log2(array_size) + 1;
    int max_levels_saved = num_levels - 1;
    int levels_saved = std::max(0,std::min(levels_saved_arg,max_levels_saved));
    int u_levels_saved = std::pow(2,levels_saved+1) - 1;

    while(true){
        if (batch_iter >= num_batches){
            break;
        }

        int batch_start = batch_starts[batch_iter];
        int batch_end = (batch_iter == num_batches-1) ? ops.size() : batch_starts[batch_iter+1];
        int batch_type = ops[batch_start][0];

        if (batch_type == UPDATE){
            for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
                const auto& op = ops[op_i];
                int i = op[1];
                int x = op[2];

                int u = i + array_size - 1;
                ST_mutexes[u].lock();
                ST[u] = combine_fn(ST[u],x);
                ST_mutexes[u].unlock();

                while (u >= u_levels_saved){
                    u = parent(u);
                    int left_child_u = leftChild(u);
                    int right_child_u = rightChild(u);
                    current_level = (int)std::log2(u + 1);
                    ST_mutexes[u].lock();
                    ST[u] = combine_fn(ST[left_child_u],ST[right_child_u]);
                    ST_mutexes[u].unlock();
                }
            }
            batch_barrier.arrive_and_wait();
            for (int level = levels_saved-1; level >= 0; level--){
                current_level = level;
                int num_nodes = std::pow(2,level);
                int start_node = num_nodes - 1;
                for (int node = 0; node < num_nodes; node += CONTIGUOUS_UPDATES_PER_THREAD * num_threads) {
                    /* 16 contiguous updates per thread ensures locality */
                    for (int node_i = node + CONTIGUOUS_UPDATES_PER_THREAD * tid;
                         node_i < std::min(node + CONTIGUOUS_UPDATES_PER_THREAD * (tid+1), num_nodes);
                         node_i ++) {
                        int u = start_node + node_i;
                        /* All of these writes will be to one or two cache lines (depending on alignment) */
                        /* All of the reads will be contiguous sections */
                        ST[u] = combine_fn(ST[leftChild(u)],ST[rightChild(u)]);
                    }
                }
                batch_barrier.arrive_and_wait();
            }
        } else if(batch_type == QUERY){
            for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
                const auto& op = ops[op_i];
                int i = op[1];
                int j = op[2];
                int local_index = op_i - batch_start;
                int result_index = queries_completed + local_index;
                int query_answer = computeSumCombine(0,i,j,0,array_size,ST,combine_fn);
                query_results[result_index][OPERATION_INDEX] = op_i;
                query_results[result_index][QUERY_ANS] = query_answer;
            }
            queries_completed += batch_end - batch_start;
        }

        batch_barrier.arrive_and_wait();
        batch_iter++;
    }
}

void runFineImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                        std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn) {
    std::vector<std::mutex> ST_mutexes(ST_size);
    std::barrier batch_barrier(num_threads);

    /* Create the threads and have each one begin execution */
    std::vector<std::thread> threads;
    for (int tid = 0; tid < num_threads; tid++) {
        /* Pass as reference so that updates occur to the array we input */
        threads.emplace_back(fineWorker, num_threads, tid, array_size, levels_saved, std::ref(ops), std::ref(ST), std::ref(query_results), std::ref(ST_mutexes),
                            std::ref(batch_barrier),batch_starts, combine_fn);
    }

    for(auto &t: threads){
        t.join();
    }
}
