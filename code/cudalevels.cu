#include <cuda_runtime.h>
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

__global__ void update_leaf_nodes(int* ST, const int* device_ops, const int batch_start, const int batch_size, const int array_size){
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
    int thread_stride = gridDim.x * blockDim.x;
    for (int op_i = thread_id + batch_start; op_i < batch_start + batch_size; op_i += thread_stride){
        int i = device_ops[3 * op_i + 1];
        int increment = device_ops[3 * op_i + 2];

        int u = i + array_size - 1;
        atomicAdd(&ST[u],increment);
    }
}

__global__ void update_interior_nodes(int* ST, const int* device_ops, const int batch_start, const int batch_size, const int array_size, const int level){
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
    int thread_stride = gridDim.x * blockDim.x;

    int num_nodes = 1 << level;
    int level_offset = num_nodes - 1;
    for (int level_index = thread_id+level_offset; level_index < num_nodes+level_offset; level_index += thread_stride){
        ST[level_index] = ST[2*level_index+1] + ST[2*level_index+2];
    }
}

void runLevelsCudaImplementation(const int num_ops, const int array_size, const int ST_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results){
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

    int ST_byte_size = ST_size * sizeof(int);

    int* device_ST;
    cudaMalloc(&device_ST,ST_byte_size);
    cudaMemset(device_ST,0,ST_byte_size);

    int* device_ops;
    int ops_byte_size = num_ops * sizeof(std::array<int,3>);
    cudaMalloc(&device_ops,ops_byte_size);
    cudaMemcpy(device_ops,&ops[0],ops_byte_size,cudaMemcpyHostToDevice);

    std::vector<int> host_ST(ST_size);
    int threads_per_block = 256;

    int last_level_ind = std::log2(array_size);

    int query_offset = 0;
    for(int batch_iter = 0; batch_iter < num_batches; batch_iter++){
        int batch_start = batch_starts[batch_iter];
        int batch_end = (batch_iter == num_batches-1) ? num_ops : batch_starts[batch_iter+1];
        int batch_type = ops[batch_start][0];
        int batch_size = batch_end - batch_start;

        int num_blocks = (batch_size + threads_per_block - 1)/threads_per_block;

        if(batch_type == UPDATE){
            update_leaf_nodes<<<num_blocks,threads_per_block>>>(device_ST,device_ops,batch_start,batch_size,array_size);
            for (int level = last_level_ind - 1; level >= 0; level--){
                update_interior_nodes<<<num_blocks,threads_per_block>>>(device_ST,device_ops,batch_start,batch_size,array_size,level);
            }
        }
        else if(batch_type == QUERY){
            cudaMemcpy(&host_ST[0],device_ST,ST_byte_size,cudaMemcpyDeviceToHost);
            
            for (int op_i = batch_start; op_i < batch_end; op_i++){
                int i = ops[op_i][1];
                int j = ops[op_i][2];
                int query_answer = computeSum(0,i,j,0,array_size,host_ST);
                query_results[query_offset] = {op_i,query_answer};
                query_offset++;
            }
        }
    }
    cudaFree(device_ST);
    cudaFree(device_ops);
}

