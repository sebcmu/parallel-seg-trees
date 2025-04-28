#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include "constants.hpp"
#include "helpers.hpp"

void runSerialImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                        std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, IntCombine combine_fn, const int combine_type) {
    int n = array_size;
    
    int query_iter = 0;
    int query_answer;
    for (int op_i = 0; op_i < num_ops; op_i++){
        auto& op = ops[op_i];
        if (op[0] == UPDATE){
            int i = op[1];
            int x = op[2];
            int u = i + n - 1;
            ST[u] = combine_fn(ST[u],x);
            while (u > 0){
                u = parent(u);
                ST[u] = combine_fn(ST[leftChild(u)],ST[rightChild(u)]);
            }
        }
        else if(op[0] == QUERY){
            int i = op[1];
            int j = op[2];
            query_answer = computeSumCombine(0,i,j,0,n,ST,combine_fn, combine_type);
            query_results[query_iter][OPERATION_INDEX] = op_i;
            query_results[query_iter][QUERY_ANS] = query_answer;
            query_iter++;
        }
    }
}