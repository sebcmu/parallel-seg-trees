#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/nonconstant/output_speedup_prefetch_levels_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

make clean
make

##### VARYING LEVELS SAVED

##### FINE PREFETCH IMPLEMENTATION, 1 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 1

##### LOCKFREE PREFETCH IMPLEMENTATION, 1 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 1
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 1

##### FINE PREFETCH IMPLEMENTATION, 2 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 2

##### LOCKFREE PREFETCH IMPLEMENTATION, 2 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 2
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 2

##### FINE PREFETCH IMPLEMENTATION, 3 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 3

##### LOCKFREE PREFETCH IMPLEMENTATION, 3 AHEAD
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 0 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 2 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 4 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 6 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 8 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 10 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 12 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 14 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 16 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 18 -p 3
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -t 32 -i 1 -c nonconst -v -l 20 -p 3