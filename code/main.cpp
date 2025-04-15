#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <array>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <chrono>
#include <atomic>

#include "constants.hpp"

void runSerialImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results);

void runCoarseImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads);

void runFineImplementation(const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads);

void runLockFreeImplementation(const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::atomic<int>* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads);

void runPrefixCudaImplementation(const int num_ops, const int array_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results);

int main(int argc, char* argv[]) {
    /* Default Parameters */
    const auto init_start = std::chrono::steady_clock::now();
    std::string mode = "serial";
    int validate = 0;
    std::string input_filename = "./inputs/testinputs/simpletest.txt";
    int num_threads = 1;
    int levels_saved = 1; /* Hyperparameter, determines when to stop using locking and instead use serial propagation up */

    /* Read Inputs */
    int opt;
    while ((opt = getopt(argc, argv, "l:f:m:t:v")) != -1) {
        switch (opt) {
            case 'l':
                levels_saved = atoi(optarg);
                break;
            case 'f':
                input_filename = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
            case 'v':
                validate = 1;
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << "./rangequery -m <mode> -f <input_filename> -t <num_threads> [-v]\n";
                return 1;
        }
    }

    // Check if required options are provided
    if (empty(input_filename) || num_threads < 1) {
        std::cerr << "Usage: " << argv[0] << "./rangequery -m <mode> -f <input_filename> -t <num_threads> [-v]\n";;
        return 1;
    }

    /* Parse Input File */
    std::ifstream fin(input_filename);

    if (!fin) {
        std::cerr << "Unable to open file: " << input_filename << ".\n";
        exit(EXIT_FAILURE);
    }

    int num_query, num_update;
    int orig_array_size, array_size;

    /* Read Array Size, Number of Updates, Number of Queries */
    fin >> orig_array_size >> num_update >> num_query;
    int num_ops = num_query + num_update;

    
    std::vector<std::array<int,3>> ops(num_ops);
    char type;
    int type_bit = -1;
    int arg1, arg2;

    /* Read Operations */
    /* Assume well-formed files */
    for (int op_i = 0; op_i < num_ops; op_i++) {
        fin >> type >> arg1 >> arg2;
        /* Update is 0, Query is 1 */
        if (type == 'u') {
            type_bit = UPDATE;
        } else if (type == 'q') {
            type_bit = QUERY;
        /* Erraneous operation check */
        } else {
            std::cout << "Unsupported operation type from operation " << op_i << "\n";
        }

        /* Erraneous update/query checks */
        /* Could be useful for if we write a script to generate test cases, want to make sure it's correct */
        if (type_bit == UPDATE) {
            if (arg1 < 0 || arg1 >= orig_array_size) {
                std::cout << "Cannot update outside original array from operation " << op_i << "\n";
            }
        } else {
            if (arg1 < 0 || arg1 > orig_array_size || arg2 < 0 || arg2 > orig_array_size) {
                std::cout << "Cannot query range outside original array from operation " << op_i << "\n";
            }
        }

        /* If nothing is wrong, proceed */
        ops[op_i][0] = type_bit;
        ops[op_i][1] = arg1;
        ops[op_i][2] = arg2;
    }

    /* Building the SegTree */
    /* Check if power of two, if not, round up (fill bits to right then add 1 to turn into 00100....0 i.e. power of 2)*/
    array_size = static_cast<int>(std::pow(2, std::ceil(std::log2(orig_array_size))));
    int ST_size = array_size * 2 - 1;
    /* need to fill padded spaces with identity element (eg sum -> 0 (current), max -> -inf, min -> inf) */

    /* Is comparing optimizations that use different array types valid? */
    std::vector<int> ST(ST_size,0);
    std::atomic<int>* ST_LF = new std::atomic<int>[ST_size];
    for (int i = 0; i < ST_size; i++){
        ST_LF[i].store(0,std::memory_order_relaxed);
    }
    
    /* Create vector query_results to check queries for correctness */
    std::vector<std::array<int,2>> query_results(num_query);

    const double init_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - init_start).count();

    /* Begin computation */
    const auto compute_start = std::chrono::steady_clock::now();
    if (mode == "serial") {
        std::cout << "[INFO] Running the serial implementation...\n";
        runSerialImplementation(num_ops, num_query, num_update, ops, ST_size, ST, array_size, orig_array_size, query_results);
    } 
    else if (mode == "coarse") {
        std::cout << "[INFO] Running the coarse-grained locking implementation...\n";
        runCoarseImplementation(num_ops, num_query, num_update, ops, ST_size, ST, array_size, orig_array_size, query_results, num_threads);
    } 
    else if (mode == "fine") {
        std::cout << "[INFO] Running the fine-grained locking implementation...\n";
        runFineImplementation(num_ops, num_query, num_update, levels_saved, ops, ST_size, ST, array_size, orig_array_size, query_results, num_threads);
    } 
    else if (mode == "lockfree"){
        std::cout << "[INFO] Running the lock-free implementation...\n";
        runLockFreeImplementation(num_ops, num_query, num_update, levels_saved, ops, ST_size, ST_LF, array_size, orig_array_size, query_results, num_threads);
    }
    else if (mode == "cuda") {
        std::cout << "[INFO] Running the CUDA prefix sum implementation...\n";
        runPrefixCudaImplementation(num_ops, array_size, ops, query_results);
    }
    else {
        std::cerr << "Error: Unknown mode \"" << mode << "\".\n";
        return 1;
    }

    const double compute_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - compute_start).count();

    std::cout << "Initialization time (sec): " << std::fixed << std::setprecision(5) << init_time << '\n';
    std::cout << "Computation time (sec): " << std::fixed << std::setprecision(5) << compute_time << '\n';
    std::cout << "Total time (sec): " << std::fixed << std::setprecision(5) << compute_time + init_time << '\n';
    

    /* CHECK queries if validate flag is on */
    if (validate){
        for (int i = 0; i < num_query; i++){
            int res;
            fin >> res;
            if (query_results[i][1] != res) {
                std::cout << "Query " << i << " is incorrect, expected: " << res << ", actual: " << query_results[i][1] << ", Operation Index: " << query_results[i][0] <<"\n";
                // break;
            }
            if (i == num_query - 1) {
                std::cout << "All query results are valid\n";
            }
        }
    }

    delete[] ST_LF;

    return 0;
}