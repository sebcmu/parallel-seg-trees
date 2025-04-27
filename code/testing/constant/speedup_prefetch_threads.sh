#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/constant/output_speedup_prefetch_threads_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

make clean
make

##### VARYING NUMBER OF THREADS, USING BEST LEVELS SAVED AS DISCOVERED

##### FINE PREFETCH IMPLEMENTATION, 1 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 1 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 2 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 4 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 8 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 16 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 5 -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 64 -i 5 -v -l 14 -p 1

##### LOCKFREE PREFETCH IMPLEMENTATION, 1 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 1 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 2 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 4 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 8 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 16 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 5 -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 64 -i 5 -v -l 12 -p 1

##### FINE PREFETCH IMPLEMENTATION, 2 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 1 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 2 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 4 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 8 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 16 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 5 -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 64 -i 5 -v -l 14 -p 2

##### LOCKFREE PREFETCH IMPLEMENTATION, 2 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 1 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 2 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 4 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 8 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 16 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 5 -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 64 -i 5 -v -l 12 -p 2

##### FINE PREFETCH IMPLEMENTATION, 3 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 1 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 2 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 4 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 8 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 16 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 5 -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 64 -i 5 -v -l 14 -p 3

##### LOCKFREE PREFETCH IMPLEMENTATION, 3 AHEAD
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 1 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 2 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 4 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 8 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 16 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 5 -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 64 -i 5 -v -l 12 -p 3