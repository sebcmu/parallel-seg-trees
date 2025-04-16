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

/* Timing code */
std::chrono::duration<double> fine_total_query_time(0.0);
std::chrono::duration<double> fine_total_levels_time(0.0);
std::chrono::duration<double> fine_total_update_time(0.0);

void fineWorker(
    int num_threads, int tid,
    const int array_size, const int levels_saved_arg,
    const std::vector<std::array<int, 3>>& ops,
    std::vector<int>& ST,
    std::vector<std::array<int,2>>& query_results,
    std::vector<std::mutex>& ST_mutexes,
    std::barrier<> &batch_barrier,
    const std::vector<int>& batch_starts)
{
    int queries_completed = 0;
    int batch_iter = 0;
    int num_batches = batch_starts.size();
    // optimization
    int num_levels = std::log2(array_size) + 1;
    int max_levels_saved = num_levels - 1;
    int levels_saved = std::min(levels_saved_arg,max_levels_saved);
    /* First x levels correspond to first 2^x-1 array elements*/
    int u_levels_saved = std::pow(2,levels_saved+1) - 1;

    while(true){
        if (batch_iter >= num_batches){
            break;
        }

        /* Timing code */
        std::chrono::steady_clock::time_point batch_start_time;
        if (tid == 0) {
            batch_start_time = std::chrono::steady_clock::now();
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
                // lock ST[u]
                ST_mutexes[u].lock();
                ST[u] += x;
                ST_mutexes[u].unlock();
                // unlock ST[u]
                while (u >= u_levels_saved){
                    // parent, leftChild, and rightChild are just mathematical functions on u and don't need locking
                    u = parent(u);
                    int left_child_u = leftChild(u);
                    int right_child_u = rightChild(u);
                    ST_mutexes[u].lock();
                    ST[u] = ST[left_child_u] + ST[right_child_u];
                    ST_mutexes[u].unlock();
                }
            }
            batch_barrier.arrive_and_wait();
            std::chrono::steady_clock::time_point levels_start_time;
            if (tid == 0){
                levels_start_time = std::chrono::steady_clock::now();
                for (int level = levels_saved-1; level >= 0; level--){
                    int num_nodes = std::pow(2,level);
                    int start_node = num_nodes - 1;
                    for (int node = 0; node < num_nodes; node++){
                        int u = start_node + node;
                        ST[u] = ST[leftChild(u)] + ST[rightChild(u)];
                    }
                }
                auto levels_end_time = std::chrono::steady_clock::now();
                fine_total_levels_time += levels_end_time - levels_start_time;
            }
        } else if(batch_type == QUERY){
            for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
                const auto& op = ops[op_i];
                int i = op[1];
                int j = op[2];
                int local_index = op_i - batch_start;
                int result_index = queries_completed + local_index;
                int query_answer = computeSum(0,i,j,0,array_size,ST);
                query_results[result_index][OPERATION_INDEX] = op_i;
                query_results[result_index][QUERY_ANS] = query_answer;
            }
            queries_completed += batch_end - batch_start;
        }
        
        batch_barrier.arrive_and_wait();
        batch_iter++;
        /* Timing code */
        if (tid == 0) {
            auto batch_end_time = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = batch_end_time - batch_start_time;
            if (batch_type == QUERY) {
                fine_total_query_time += elapsed;
            } else if (batch_type == UPDATE) {
                fine_total_update_time += elapsed;
            }
        }
    }
}

void runFineImplementation(const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                        std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads) {  
    /* Calculate batch_starts for continuous batches of updates and queries */
    std::vector<int> batch_starts;
    batch_starts.push_back(0);
    int prev_type = ops[0][0];
    int type = -1;
    for (int op_i = 1; op_i < num_ops; op_i ++){
        type = ops[op_i][0];
        if (type != prev_type){
            batch_starts.push_back(op_i);
        }
        prev_type = type;
    }

    std::vector<std::mutex> ST_mutexes(ST_size);
    std::barrier batch_barrier(num_threads);

    /* Create the threads and have each one begin execution */
    std::vector<std::thread> threads;
    for (int tid = 0; tid < num_threads; tid++) {
        /* Pass as reference so that updates occur to the array we input */
        threads.emplace_back(fineWorker, num_threads, tid, array_size, levels_saved, std::ref(ops), std::ref(ST), std::ref(query_results), std::ref(ST_mutexes),
                            std::ref(batch_barrier),std::ref(batch_starts));
    }

    for(auto &t: threads){
        t.join();
    }

    /* Timing code */
    std::cout << "Total query time: " << fine_total_query_time.count() << " sec" << '\n';
    std::cout << "Total levels time: " << fine_total_levels_time.count() << " sec" << '\n';
    std::cout << "Total update time: " << fine_total_update_time.count() << " sec" << '\n';
}