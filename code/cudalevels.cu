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

__device__ int deviceComputeSum(int u, int i, int j, int L, int R, const int* ST) {
    if (i <= L && R <= j) {
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return deviceComputeSum(2*u+2,i,j,mid,R,ST);
        }
        else if (j <= mid){
            return deviceComputeSum(2*u+1,i,j,L,mid,ST);
        }
        else{
            return deviceComputeSum(2*u+1,i,j,L,mid,ST) + deviceComputeSum(2*u+2,i,j,mid,R,ST);
        }
    }
}

__global__ void process_queries(const int* ST, const int* device_ops, int2* device_query_results, int batch_start, int batch_size, int array_size) {
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
    if (thread_id >= batch_size) return;

    int op_i = batch_start + thread_id;
    int i = device_ops[3*op_i+1];
    int j = device_ops[3*op_i+2];

    int sum = deviceComputeSum(0, i, j, 0, array_size, ST);
    device_query_results[thread_id] = make_int2(op_i, sum);
}

std::chrono::duration<double> cudalevels_total_query_time(0.0);
std::chrono::duration<double> cudalevels_total_levels_time(0.0);
std::chrono::duration<double> cudalevels_total_update_time(0.0);

void runLevelsCudaImplementation(const int num_ops, const int num_query, const int num_update, const int array_size, const int ST_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results){
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

    int max_batch_size = 0;
    for (int b = 0; b < num_batches; ++b) {
        int start = batch_starts[b];
        int end   = (b+1 < (int)batch_starts.size() ? batch_starts[b+1] : num_ops);
        max_batch_size = std::max(max_batch_size, end - start);
    }

    int* device_ST;
    int ST_byte_size = ST_size * sizeof(int);
    cudaMalloc(&device_ST,ST_byte_size);
    cudaMemset(device_ST,0,ST_byte_size);

    int* device_ops;
    int ops_byte_size = num_ops * sizeof(std::array<int,3>);
    cudaMalloc(&device_ops,ops_byte_size);
    cudaMemcpy(device_ops,&ops[0],ops_byte_size,cudaMemcpyHostToDevice);

    int2* device_query_results_buffer;
    cudaMalloc(&device_query_results_buffer, max_batch_size * sizeof(int2));

    int2* host_query_results_buffer;
    cudaMallocHost(&host_query_results_buffer, max_batch_size * sizeof(int2));
    
    int threads_per_block = 256;

    int last_level_ind = std::log2(array_size);

    int query_offset = 0;
    for(int batch_iter = 0; batch_iter < num_batches; batch_iter++){
        int batch_start = batch_starts[batch_iter];
        int batch_end = (batch_iter == num_batches-1) ? num_ops : batch_starts[batch_iter+1];
        int batch_type = ops[batch_start][0];
        int batch_size = batch_end - batch_start;

        int num_blocks = (batch_size + threads_per_block - 1)/threads_per_block;

        const auto batch_start_time = std::chrono::steady_clock::now();

        if(batch_type == UPDATE){
            update_leaf_nodes<<<num_blocks,threads_per_block>>>(device_ST,device_ops,batch_start,batch_size,array_size);
            const auto levels_start_time = std::chrono::steady_clock::now();
            for (int level = last_level_ind - 1; level >= 0; level--){
                update_interior_nodes<<<num_blocks,threads_per_block>>>(device_ST,device_ops,batch_start,batch_size,array_size,level);
            }
            auto levels_end_time = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = levels_end_time - levels_start_time;
            cudalevels_total_levels_time += elapsed;
        }
        else if(batch_type == QUERY){
            process_queries<<<num_blocks, threads_per_block>>>(
                device_ST,
                device_ops,
                device_query_results_buffer,
                batch_start,
                batch_size,
                array_size
            );

            cudaMemcpy(
                host_query_results_buffer,
                device_query_results_buffer,
                batch_size * sizeof(int2),
                cudaMemcpyDeviceToHost
            );
            for (int t = 0; t < batch_size; ++t) {
                query_results[query_offset + t] = {host_query_results_buffer[t].x, host_query_results_buffer[t].y};
            }
            query_offset += batch_size;
        }
        auto batch_end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = batch_end_time - batch_start_time;
        if (batch_type == UPDATE) {
            cudalevels_total_update_time += elapsed;
        } else {
            cudalevels_total_query_time += elapsed;
        }
    }

    /* Timing code */
    std::cout << "Total query time: " << cudalevels_total_query_time.count() << " sec" << '\n';
    std::cout << "Total levels time: " << cudalevels_total_levels_time.count() << " sec" << '\n';
    std::cout << "Total update time: " << cudalevels_total_update_time.count() << " sec" << '\n';

    cudaFree(device_ST);
    cudaFree(device_ops);
    cudaFree(device_query_results_buffer);
    cudaFreeHost(host_query_results_buffer);    
}

