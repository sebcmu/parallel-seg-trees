#!/bin/bash
#SBATCH -p RM
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH -o testing/constant/output_per_thread_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc
module load intel-vtune/2023.2.0

cd ~/parallel-seg-trees/code

make clean
make

# Low thread counts
vtune -collect threading -result-dir vtune_tmp ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 5 -v
vtune -report hotspots -r vtune_tmp -format=csv -report-output=vtune/constant/small_t4_fine.csv
rm -rf vtune_tmp

# ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 5 -v

# ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 5 -v
# ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/small_n_65536_o_262144_c_1024_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 5 -v

# # Medium thread counts
# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 5 -v
# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 5 -v

# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 5 -v
# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/medium_n_262144_o_262144_c_2048_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 5 -v

# # High thread counts
# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 4 -i 5 -v
# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m fine -t 64 -i 5 -v

# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 4 -i 5 -v
# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 16 -i 5 -v
# ./rangequery -f ./inputs/final/constant/large_n_1048576_o_1048576_c_4096_r_20_q_0.50_p_sum.txt -m lockfree -t 64 -i 5 -v