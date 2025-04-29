#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/nonconstant/output_timing_qprop%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

make clean
make

# COMPARING COMP TIMES ONLY

# SERIAL
./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m serial -i 5 -v
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m serial -i 5 -v

# FINE, 16 threads was best in speedup test

./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m fine -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m fine -i 5 -v -t 64

# FINE Prefetch, best config: 16 threads, 14 levels saved, aggression = 1
./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m fine_prefetch -i 5 -v -t 64 -l 8 -p 1

# FINE Padded, 16 threads was best

./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m fine_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m fine_padded -i 5 -v -t 64

# LF, 32 threads was best in speedup test
./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m lockfree -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m lockfree -i 5 -v -t 64

# LF Prefetch, best config: 32 threads, 12 levels saved, aggression = 2
./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m lockfree_prefetch -i 5 -v -t 64 -l 8 -p 1

# LF Padded, 32 threads was marginally worse than 16 threads, but consistency > marginal difference
./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m lockfree_padded -i 5 -v -t 64
./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m lockfree_padded -i 5 -v -t 64