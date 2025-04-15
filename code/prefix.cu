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

__global__ void process_update_batch(int* prefix_sum, const int* device_ops, const int batch_start, const int batch_size, const int prefix_sum_size){
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
    int warp_id = thread_id/32;
    int warp_stride = (gridDim.x * blockDim.x + 31) / 32;
    int lane_id = threadIdx.x % 32;
    int lane_stride = 32;
    /* Each warp takes an update, uses lanes to quickly increment positions [i+1,prefix_sum_size) by x*/
    for (int i = warp_id + batch_start; i<batch_start+batch_size; i+=warp_stride){
        int start_index = device_ops[3 * i + 1] + 1;
        int increment = device_ops[3 * i + 2];
        
        /*Each lane takes an increment*/
        for (int j = start_index + lane_id; j < prefix_sum_size; j += lane_stride){
            atomicAdd(&prefix_sum[j],increment);
        }
    }
}

void runPrefixCudaImplementation(const int num_ops, const int array_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results){
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

    /* PS[i] = prefix sum up to but not including i, so need array_size + 1 spaces to store all info (PS[0] = 0 trivially and does not store info) */
    int prefix_sum_size = array_size + 1;
    int prefix_sum_byte_size = prefix_sum_size * sizeof(int);

    int* device_prefix_sum;
    cudaMalloc(&device_prefix_sum,prefix_sum_byte_size);
    cudaMemset(device_prefix_sum,0,prefix_sum_byte_size);

    int* device_ops;
    int ops_byte_size = num_ops * sizeof(std::array<int,3>);
    cudaMalloc(&device_ops,ops_byte_size);
    cudaMemcpy(device_ops,&ops[0],ops_byte_size,cudaMemcpyHostToDevice);

    std::vector<int> host_prefix_sum(prefix_sum_size);
    int threads_per_block = 256;

    int query_offset = 0;
    for(int batch_iter = 0; batch_iter < num_batches; batch_iter++){
        int batch_start = batch_starts[batch_iter];
        int batch_end = (batch_iter == num_batches-1) ? num_ops : batch_starts[batch_iter+1];
        int batch_type = ops[batch_start][0];
        int batch_size = batch_end - batch_start;

        int num_blocks = (batch_size + threads_per_block - 1)/threads_per_block;

        if(batch_type == UPDATE){
            process_update_batch<<<num_blocks,threads_per_block>>>(device_prefix_sum,device_ops,batch_start,batch_size,prefix_sum_size);
            cudaDeviceSynchronize();
        }
        else if(batch_type == QUERY){
            cudaMemcpy(&host_prefix_sum[0],device_prefix_sum,prefix_sum_byte_size,cudaMemcpyDeviceToHost);
            for (int op_i = batch_start; op_i < batch_end; op_i++){
                int l = ops[op_i][1];
                int r = ops[op_i][2];
                int query_answer = host_prefix_sum[r] - host_prefix_sum[l];
                query_results[query_offset] = {op_i,query_answer};
                query_offset++;
            }
        }
    }
    cudaFree(device_prefix_sum);
    cudaFree(device_ops);
}

