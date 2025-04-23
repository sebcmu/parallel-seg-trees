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
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, IntCombine combine_fn);

void runCoarseImplementation(const int num_ops, const int num_query, const int num_update, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn);

void runFineImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn);

void runFinePaddedImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    PaddedInt* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn);

void runFinePrefetchImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::vector<int>& ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn, const int prefetch_levels);

void runLockFreeImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::atomic<int>* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn);

void runLockFreePaddedImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    PaddedAtomicInt* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn);

void runLockFreePrefetchImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int levels_saved, const std::vector<std::array<int, 3>>& ops, const int ST_size,
    std::atomic<int>* ST, const int array_size, const int orig_array_size, std::vector<std::array<int,2>>& query_results, const int num_threads, IntCombine combine_fn, const int prefetch_levels);

void runCudaPrefixImplementation(const std::vector<int>& batch_starts, const int num_ops, const int array_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results);

void runCudaLevelsImplementation(const std::vector<int>& batch_starts, const int num_ops, const int num_query, const int num_update, const int array_size, const int ST_size, const std::vector<std::array<int, 3>>& ops,std::vector<std::array<int, 2>>& query_results, const int combine_type, IntCombine combine_fn);

std::tuple<double, double> run(const int num_threads, const int orig_array_size, const int num_query, const int num_update, const int num_ops, const std::vector<std::array<int, 3>>& ops, const std::vector<int>& batch_starts, IntCombine combine_fn, const int combine_type, const int prefetch_levels, const int levels_saved, std::string mode, const int validate, const std::vector<int>& expected_query_results) {
    const auto run_init_start = std::chrono::steady_clock::now();

    /* Building the SegTree */
    /* Check if power of two, if not, round up (fill bits to right then add 1 to turn into 00100....0 i.e. power of 2)*/
    int array_size = static_cast<int>(std::pow(2, std::ceil(std::log2(orig_array_size))));
    int ST_size = array_size * 2 - 1;
    /* need to fill padded spaces with identity element (eg sum -> 0 (current), max -> -inf, min -> inf) */

    /* Is comparing optimizations that use different array types valid? */
    std::vector<int> ST(ST_size,0);
    std::atomic<int>* ST_LF = new std::atomic<int>[ST_size];
    for (int i = 0; i < ST_size; i++){
        ST_LF[i].store(0,std::memory_order_relaxed);
    }
    PaddedAtomicInt* ST_LF_PAD = new PaddedAtomicInt[ST_size];
    for (int i = 0; i < ST_size; i++){
        ST_LF_PAD[i].value.store(0,std::memory_order_relaxed);
    }
    PaddedInt* ST_F_PAD = new PaddedInt[ST_size];
    for (int i = 0; i < ST_size; i++){
        ST_F_PAD[i].value = 0;
    }
    
    /* Create vector query_results to check queries for correctness */
    std::vector<std::array<int,2>> query_results(num_query);

    const double run_init_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - run_init_start).count();

    /* Begin computation */
    const auto run_compute_start = std::chrono::steady_clock::now();
    if (mode == "serial") {
        std::cout << "[INFO] Running the serial implementation...\n";
        runSerialImplementation(num_ops, num_query, num_update, ops, ST_size, ST, array_size, orig_array_size, query_results, combine_fn);
    } 
    else if (mode == "coarse") {
        std::cout << "[INFO] Running the coarse-grained locking implementation...\n";
        runCoarseImplementation(num_ops, num_query, num_update, ops, ST_size, ST, array_size, orig_array_size, query_results, num_threads, combine_fn);
    } 
    else if (mode == "fine") {
        std::cout << "[INFO] Running the fine-grained locking implementation...\n";
        runFineImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST, array_size, orig_array_size, query_results, num_threads, combine_fn);
    } 
    else if (mode == "fine_padded") {
        std::cout << "[INFO] Running the fine-grained locking padded implementation...\n";
        runFinePaddedImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST_F_PAD, array_size, orig_array_size, query_results, num_threads, combine_fn);
    } 
    else if (mode == "fine_prefetch") {
        std::cout << "[INFO] Running the fine-grained locking prefetch implementation...\n";
        runFinePrefetchImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST, array_size, orig_array_size, query_results, num_threads, combine_fn, prefetch_levels);
    } 
    else if (mode == "lockfree"){
        std::cout << "[INFO] Running the lock-free implementation...\n";
        runLockFreeImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST_LF, array_size, orig_array_size, query_results, num_threads, combine_fn);
    }
    else if (mode == "lockfree_padded"){
        std::cout << "[INFO] Running the lock-free padded implementation...\n";
        runLockFreePaddedImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST_LF_PAD, array_size, orig_array_size, query_results, num_threads, combine_fn);
    }
    else if (mode == "lockfree_prefetch"){
        std::cout << "[INFO] Running the lock-free prefetch implementation...\n";
        runLockFreePrefetchImplementation(batch_starts, num_ops, num_query, num_update, levels_saved, ops, ST_size, ST_LF, array_size, orig_array_size, query_results, num_threads, combine_fn, prefetch_levels);
    }
    else if (mode == "cudaprefix") {
        /* In a prefix sum implementation, we use a "reverse" function (subtraction) to compute queries based off the psums array */
        /* There is no subtraction equivalent for the other combine functions we use, so we leave out the generalization in prefixSums */
        std::cout << "[INFO] Running the CUDA prefix sum implementation...\n";
        runCudaPrefixImplementation(batch_starts, num_ops, array_size, ops, query_results);
    }
    else if (mode == "cudalevels") {
        std::cout << "[INFO] Running the CUDA level update implementation...\n";
        runCudaLevelsImplementation(batch_starts, num_ops, num_query, num_update, array_size, ST_size, ops, query_results, combine_type, combine_fn);
    }
    else {
        std::cerr << "Error: Unknown mode \"" << mode << "\".\n";
        return {0.0, 0.0};
    }

    const double run_compute_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - run_compute_start).count();

    std::cout << "Init (sec): " << std::fixed << std::setprecision(5) << run_init_time << '\n';
    std::cout << "Comp (sec): " << std::fixed << std::setprecision(5) << run_compute_time << '\n';
    std::cout << "Tot  (sec): " << std::fixed << std::setprecision(5) << run_compute_time + run_init_time << '\n';
    

    /* CHECK queries if validate flag is on */
    if (validate){
        for (int i = 0; i < num_query; i++){
            if (query_results[i][1] != expected_query_results[i]) {
                std::cout << "Query " << i << " is incorrect, expected: " << expected_query_results[i] << ", actual: " << query_results[i][1] << ", Operation Index: " << query_results[i][0] <<"\n";
                // break;
            }
            if (i == num_query - 1) {
                std::cout << "All query results are valid\n";
            }
        }
    }

    delete[] ST_LF;
    delete[] ST_LF_PAD;
    delete[] ST_F_PAD;

    return {run_init_time, run_compute_time};
}

int main(int argc, char* argv[]) {
    /* Default Parameters */
    const auto init_start = std::chrono::steady_clock::now();
    int num_runs = 1;
    std::string mode = "serial";
    int validate = 0;
    std::string input_filename = "./inputs/testinputs/simpletest.txt";
    int num_threads = 1;
    int levels_saved = 1; /* Hyperparameter, determines when to stop using locking and instead use serial propagation up */
    std::string combine_fn_str = "sum";
    IntCombine combine_fn;
    int combine_type;
    int prefetch_levels = 0; /* Hyperparameter, determines how far to prefetch upwards in fine_prefetch and lockfree_prefetch */

    /* Read Inputs */
    int opt;
    while ((opt = getopt(argc, argv, "i:p:l:f:m:t:c:v")) != -1) {
        switch (opt) {
            case 'l': levels_saved = atoi(optarg);              break;
            case 'f': input_filename = optarg;                  break;
            case 'm': mode = optarg;                            break;
            case 'v': validate = 1;                             break;
            case 't': num_threads = atoi(optarg);               break;
            case 'c': combine_fn_str = optarg;                  break;
            case 'p': prefetch_levels = atoi(optarg);           break;
            case 'i': num_runs = atoi(optarg);                  break;
            default:
                std::cerr << "Usage: " << argv[0] << "./rangequery -m <mode> -f <input_filename> -t <num_threads> [-v] [-l <levels_saved>] [-c <combine_fn_str>] [-p <prefetch_levels>] [-i <iterations>] \n";
                return 1;
        }
    }

    /* Check if required options are provided */ 
    if (empty(input_filename) || num_threads < 1) {
        std::cerr << "Usage: " << argv[0] << "./rangequery -m <mode> -f <input_filename> -t <num_threads> [-v] [-l <levels_saved>] [-c <combine_fn_str>] [-p <prefetch_levels>] [-i <iterations>] \n";
        return 1;
    }

    /* Set Combine Function*/
    /* Since cuda cannot take in std::function arguments, must pass a combine type. If adding types of combine functions, must update cudalevels.cu and constants.hpp */
    if(combine_fn_str == "sum"){
        combine_fn = [](int a, int b){return a+b;};
        combine_type = COMBINE_SUM_FLAG;
    }
    else if(combine_fn_str == "max"){
        combine_fn = [](int a, int b){return std::max(a,b);};
        combine_type = COMBINE_MAX_FLAG;
    }
    else if(combine_fn_str == "min"){
        combine_fn = [](int a, int b){return std::min(a,b);};
        combine_type = COMBINE_MIN_FLAG;
    }
    else{
        /* Default Case If No Match on Combine Function */
        combine_fn = [](int a, int b){return a+b;};
        combine_type = COMBINE_SUM_FLAG;
        std::cout << "Combine function input: " << combine_fn_str <<  " did not match an available option. Using sum.";
    }


    /* Parse Input File */
    std::ifstream fin(input_filename);

    if (!fin) {
        std::cerr << "Unable to open file: " << input_filename << ".\n";
        exit(EXIT_FAILURE);
    }

    int num_query, num_update;
    int orig_array_size;

    /* Read Array Size, Number of Updates, Number of Queries */
    fin >> orig_array_size >> num_update >> num_query;
    int num_ops = num_query + num_update;

    
    std::vector<std::array<int,3>> ops(num_ops);
    char type;
    int type_bit = -1;
    int arg1, arg2;

    std::vector<int> batch_starts;
    batch_starts.push_back(0);
    int prev_type = ops[0][0];

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

        /* Populate batch starts */
        if (op_i >= 1) {
            if (type_bit != prev_type){
                batch_starts.push_back(op_i);
            }
            prev_type = type_bit;
        }

        /* If nothing is wrong, proceed */
        ops[op_i][0] = type_bit;
        ops[op_i][1] = arg1;
        ops[op_i][2] = arg2;
    }

    std::vector<int> expected_query_results(num_query);

    if (validate){
        for (int i = 0; i < num_query; i++){
            int res;
            fin >> res;
            expected_query_results[i] = res;
        }
    }

    const double main_init_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - init_start).count();

    double total_run_init = 0.0, total_run_comp = 0.0;
    for (int i = 0; i < num_runs; i++) {
        auto [run_init_time, run_comp_time] = run(num_threads, orig_array_size, num_query, num_update, num_ops, ops, batch_starts, combine_fn, combine_type, prefetch_levels, levels_saved, mode, validate, expected_query_results);
        total_run_init += run_init_time;
        total_run_comp += run_comp_time;
    }

    double total_init = total_run_init + main_init_time;

    std::cout << "\n=== AVERAGED OVER " << num_runs << " RUNS ===\n";
    std::cout << "Avg Init (sec): " << std::fixed << std::setprecision(5) << total_init / num_runs << '\n';
    std::cout << "Avg Comp (sec): " << std::fixed << std::setprecision(5) << total_run_comp / num_runs << '\n';
    std::cout << "Avg Total (sec): " << std::fixed << std::setprecision(5) << (total_init + total_run_comp) / num_runs << '\n';

    return 0;
}