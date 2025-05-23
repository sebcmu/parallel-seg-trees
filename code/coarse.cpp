#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>
#include "constants.hpp"
#include "helpers.hpp"

std::mutex ST_mutex;
std::atomic<int> coarse_op_iter(0);
std::atomic<int> coarse_query_iter(0);

/* We change this dynamic assignment of operations to a static assignment of operations when going from coarse-grained to fine-grained locking */
/* This allowed us to remove the atomic variables of op_iter and query_result_iter in the FGL implementation */
/* We don't care about improving this here because the CGL implementation will always be slow */
void coarseWorker(const int num_ops, const int array_size, const std::vector<std::array<int, 3>>& ops, std::vector<int>& ST, std::vector<std::array<int,2>>& query_results, IntCombine combine_fn, const int combine_type){
    int query_answer;

    while(true){
        /* Acquire lock over the whole operation */
        std::lock_guard<std::mutex> lock(ST_mutex);

        int op_i = coarse_op_iter.fetch_add(1);
        if (op_i >= num_ops){
            break;
        }

        auto& op = ops[op_i];
        if (op[0] == UPDATE){
            int i = op[1];
            int x = op[2];
            int u = i + array_size - 1;
            ST[u] = combine_fn(ST[u],x);
            while (u > 0){
                u = parent(u);
                ST[u] = combine_fn(ST[leftChild(u)],ST[rightChild(u)]);
            }
        }
        else if(op[0] == QUERY){
            int i = op[1];
            int j = op[2];
            query_answer = computeSumCombine(0,i,j,0,array_size,ST,combine_fn, combine_type);
            query_results[coarse_query_iter][OPERATION_INDEX] = op_i;
            query_results[coarse_query_iter][QUERY_ANS] = query_answer;
            coarse_query_iter.fetch_add(1);
        }
    }
}

void runCoarseImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                        std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn, const int combine_type) {
    /* Reset atomic counters */
    coarse_op_iter = 0;
    coarse_query_iter = 0;

    /* Create the threads and have each one begin execution */
    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; t++) {
        /* Pass as reference so that updates occur to the array we input */
        threads.emplace_back(coarseWorker, num_ops, array_size, std::ref(ops), std::ref(ST), std::ref(query_results), combine_fn, combine_type);
    }

    for (auto &t : threads) {
        t.join();
    }
}