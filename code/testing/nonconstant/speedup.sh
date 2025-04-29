#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/nonconstant/output_speedup_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

make clean
make

##### FINE IMPLEMENTATION
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 2 -c nonconst -v

./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 2 -c nonconst -v

./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 2 -c nonconst -v

##### LOCKFREE IMPLEMENTATION
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 2 -c nonconst -v

./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 2 -c nonconst -v

./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 1 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 2 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 8 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 32 -i 2 -c nonconst -v
./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 2 -c nonconst -v