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

void lockFreePrefetchWorker(
    int num_threads, int tid,
    const int array_size, const int levels_saved_arg,
    const std::vector<std::array<int, 3>>& ops,
    std::atomic<int>* ST,
    std::vector<std::array<int,2>>& query_results,
    std::barrier<> &batch_barrier,
    const std::vector<int>& batch_starts,
    IntCombine combine_fn,
    const int prefetch_levels)
{
    int queries_completed = 0;
    int batch_iter = 0;
    int num_batches = batch_starts.size();
    // optimization
    int num_levels = std::log2(array_size) + 1;
    int max_levels_saved = num_levels - 1;
    int levels_saved = std::min(levels_saved_arg,max_levels_saved);
    int u_levels_saved = std::pow(2,levels_saved+1) - 1;

    int expected, desired;

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

                /* Perform update with CAS */
                do {
                    expected = ST[u].load(std::memory_order_relaxed);
                    desired = combine_fn(expected,x);
                } while (!ST[u].compare_exchange_weak(expected, desired, std::memory_order_relaxed));

                while (u >= u_levels_saved){
                    u = parent(u);
                    int left_child_u = leftChild(u);
                    int right_child_u = rightChild(u);
                    
                    /* Prefetch code */
                    if (prefetch_levels > 0){
                        int prefetch_u = u;
                        for(int prefetch_index = 0; prefetch_index < prefetch_levels; prefetch_index++){
                            prefetch_u = parent(prefetch_u);
                        }
                        __builtin_prefetch(&ST[prefetch_u],1,1);
                    }

                    /* Perform update with CAS */
                    int left_val, right_val;
                    do {
                        expected = ST[u].load(std::memory_order_relaxed);
                        left_val = ST[left_child_u].load(std::memory_order_relaxed);
                        right_val = ST[right_child_u].load(std::memory_order_relaxed);
                        desired = combine_fn(left_val,right_val);
                        if (ST[u].compare_exchange_weak(expected, desired, std::memory_order_relaxed)) {
                            /* Succeed, but check if the children were changed by another thread after they were read and used for update by calling thread */
                            /* This isn't needed in FGL because if a child(u) is made incorrect by T1 while T0 tries to update u, T1 will correct u after */
                            /* In lock free, multiple threads could move through u at a time (won't happen with locks), so updates need a guarantee on consistency */
                            int left_val_after = ST[left_child_u].load(std::memory_order_relaxed);
                            int right_val_after = ST[right_child_u].load(std::memory_order_relaxed);
                            if (left_val == left_val_after && right_val == right_val_after) {
                                break;
                            }
                        }
                    } while (true);
                }
            }
            batch_barrier.arrive_and_wait();
            if (tid == 0){
                for (int level = levels_saved-1; level >= 0; level--){
                    int num_nodes = std::pow(2,level);
                    int start_node = num_nodes - 1;
                    for (int node = 0; node < num_nodes; node++){
                        int u = start_node + node;
                        ST[u].store(
                            combine_fn(ST[leftChild(u)].load(std::memory_order_relaxed),ST[rightChild(u)].load(std::memory_order_relaxed)),
                            std::memory_order_relaxed
                        );
                    }
                }
            }
        } else if (batch_type == QUERY){
            for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
                const auto& op = ops[op_i];
                int i = op[1];
                int j = op[2];
                int local_index = op_i - batch_start;
                int result_index = queries_completed + local_index;
                int query_answer = lockFreeComputeSumCombine(0,i,j,0,array_size,ST,combine_fn);
                query_results[result_index][OPERATION_INDEX] = op_i;
                query_results[result_index][QUERY_ANS] = query_answer;
            }
            queries_completed += batch_end - batch_start;
        }

        
        batch_barrier.arrive_and_wait();
        batch_iter++;
    }
}

void runLockFreePrefetchImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                    std::atomic<int>* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn, const int prefetch_levels) {  

    std::barrier batch_barrier(num_threads);

    /* Create the threads and have each one begin execution */
    std::vector<std::thread> threads;
    for (int tid = 0; tid < num_threads; tid++) {
        /* Pass as reference so that updates occur to the array we input */
        threads.emplace_back(lockFreePrefetchWorker, num_threads, tid, array_size, levels_saved, std::ref(ops), std::ref(ST), std::ref(query_results),
                            std::ref(batch_barrier),std::ref(batch_starts),combine_fn,prefetch_levels);
    }

    for(auto &t: threads){
        t.join();
    }
}