#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include "constants.hpp"

int parent(const int index){
    return (index - 1)/2;
}
int leftChild(const int index){
    return 2*index + 1;
}
int rightChild(const int index){
    return 2*index + 2;
}
int computeSum(const int u, const int i, const int j, const int L, const int R, const std::vector<int>& ST){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return computeSum(rightChild(u),i,j,mid,R,ST);
        }
        else if (j <= mid){
            return computeSum(leftChild(u),i,j,L,mid,ST);
        }
        else{
            return computeSum(leftChild(u),i,j,L,mid,ST) + computeSum(rightChild(u),i,j,mid,R,ST);
        }
    }
}
void runSerialImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
                        std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results) {
    int n = array_size;
    
    int query_iter = 0;
    int query_answer;
    for (int op_i = 0; op_i < num_ops; op_i++){
        auto& op = ops[op_i];
        if (op[0] == UPDATE){
            int i = op[1];
            int x = op[2];
            int u = i + n - 1;
            ST[u] = x;
            while (u > 0){
                u = parent(u);
                ST[u] = ST[leftChild(u)] + ST[rightChild(u)];
            }
        }
        else if(op[0] == QUERY){
            int i = op[1];
            int j = op[2];
            query_answer = computeSum(0,i,j,0,n,ST);
            query_results[query_iter][OPERATION_INDEX] = op_i;
            query_results[query_iter][QUERY_ANS] = query_answer;
            query_iter++;
        }
    }
}