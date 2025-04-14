#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "constants.hpp"
#include "helpers.hpp"

void fineUpdateWorker(const int num_threads, const int tid, const int batch_start, const int batch_end, const int array_size, const std::vector<std::array<int, 3>>& ops, std::vector<int>& ST, std::vector<std::array<int,2>>& query_results, std::vector<std::mutex>& ST_mutexes){
    // std::cout << "Thread: " << tid << ", Processing updates: " << batch_start << " to " << batch_end << "\n";
    for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
        auto& op = ops[op_i];
        int i = op[1];
        int x = op[2];
        
        int u = i + array_size - 1;
        // lock ST[u]
        ST_mutexes[u].lock();
        ST[u] += x;
        ST_mutexes[u].unlock();
        // unlock ST[u]
        while (u > 0){
            // parent, leftChild, and rightChild are just mathematical functions on u and don't need locking
            u = parent(u);
            int left_child_u = leftChild(u);
            int right_child_u = rightChild(u);
            // lock ST[u], ST[left_child_u], ST[right_child_u]
            ST_mutexes[u].lock();
            ST_mutexes[left_child_u].lock();
            ST_mutexes[right_child_u].lock();
            ST[u] = ST[left_child_u] + ST[right_child_u];
            // unlock ST[u], ST[left_child_u], ST[right_child_u]
            ST_mutexes[u].unlock();
            ST_mutexes[left_child_u].unlock();
            ST_mutexes[right_child_u].unlock();
        }
    }
}

void fineQueryWorker(const int query_offset, const int num_threads, const int tid, const int batch_start, const int batch_end, const int array_size, const std::vector<std::array<int, 3>>& ops, std::vector<int>& ST, std::vector<std::array<int,2>>& query_results){
    // std::cout << "Thread: " << tid << ", Processing queries: " << batch_start << " to " << batch_end << "\n";
    int query_answer, local_index, result_index, i, j;

    for (int op_i = batch_start + tid; op_i < batch_end; op_i += num_threads) {
        auto& op = ops[op_i];
        i = op[1];
        j = op[2];
        local_index = op_i - batch_start;
        result_index = query_offset + local_index;
        query_answer = computeSum(0,i,j,0,array_size,ST);
        query_results[result_index][OPERATION_INDEX] = op_i;
        query_results[result_index][QUERY_ANS] = query_answer;
    }
}

void runFineImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
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
    int num_batches = batch_starts.size();
    /* Could potentially move this code into initialization */

    int query_offset = 0;
    std::vector<std::mutex> ST_mutexes(ST_size);
    std::vector<int> query_answers;

    /* Create the threads and have each one begin execution */
    std::vector<std::thread> threads;
    for (int batch_iter = 0; batch_iter < num_batches; batch_iter++){
        /* Batch start should always be the same as fine_op_iter, just need to pass in batch end to worker functions*/
        /* Reset fine_op_iter at each batch_start because multiple threads could increment fine_op_iter at once */
        int batch_start = batch_starts[batch_iter];
        int batch_end = batch_iter == num_batches - 1 ? num_ops : batch_starts[batch_iter + 1];
        int num_necessary_threads = std::min(num_threads,batch_end - batch_start);
        int batch_type = ops[batch_start][0];

        if (batch_type == UPDATE){
            for (int t = 0; t < num_necessary_threads; t++) {
                /* Pass as reference so that updates occur to the array we input */
                threads.emplace_back(fineUpdateWorker, num_threads, t, batch_start, batch_end, array_size, std::ref(ops), std::ref(ST), std::ref(query_results), std::ref(ST_mutexes));
            }
        }
        else if(batch_type == QUERY){
            for (int t = 0; t < num_necessary_threads; t++) {
                /* Pass as reference so that updates occur to the array we input */
                threads.emplace_back(fineQueryWorker, query_offset, num_threads, t, batch_start, batch_end, array_size, std::ref(ops), std::ref(ST), std::ref(query_results));
            }
        }

        /* This join explicitly synchronizes the threads ensuring all updates have finished before a query batch
         * and all queries have finished before an update batch begins. */
        /* However, this is likely very slow because we have to create and delete threads each batch
         * but there's probably some way to avoid this. */
        for (auto &t : threads) {
            t.join();
        }
        threads.clear();

        if (batch_type == QUERY) {
            query_offset += batch_end - batch_start;
        }
    }
}
