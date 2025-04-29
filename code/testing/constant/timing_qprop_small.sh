#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/constant/output_timing_qprop_small_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

make clean
make

# COMPARING COMP TIMES ONLY

# SERIAL
./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m serial -i 5 -v
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m serial -i 5 -v

# FINE, 16 threads was best in speedup test

./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m fine -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m fine -i 5 -v -t 16

# FINE Prefetch, best config: 16 threads, 14 levels saved, aggression = 1
./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m fine_prefetch -i 5 -v -t 16 -l 12 -p 1

# FINE Padded, 16 threads was best

./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m fine_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m fine_padded -i 5 -v -t 16

# LF, 32 threads was best in speedup test
./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m lockfree -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m lockfree -i 5 -v -t 16

# LF Prefetch, best config: 32 threads, 12 levels saved, aggression = 2
./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m lockfree_prefetch -i 5 -v -t 16 -l 12 -p 1

# LF Padded, 32 threads was marginally worse than 16 threads, but consistency > marginal difference
./rangequery -f ./inputs/final/constant/qprop_small_a_n_65536_o_262144_c_1024_r_20_q_0.10_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_b_n_65536_o_262144_c_1024_r_20_q_0.20_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_c_n_65536_o_262144_c_1024_r_20_q_0.30_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_d_n_65536_o_262144_c_1024_r_20_q_0.40_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_e_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_f_n_65536_o_262144_c_1024_r_20_q_0.60_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_g_n_65536_o_262144_c_1024_r_20_q_0.70_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_h_n_65536_o_262144_c_1024_r_20_q_0.80_p_sum.txt -m lockfree_padded -i 5 -v -t 16
./rangequery -f ./inputs/final/constant/qprop_small_i_n_65536_o_262144_c_1024_r_20_q_0.90_p_sum.txt -m lockfree_padded -i 5 -v -t 16