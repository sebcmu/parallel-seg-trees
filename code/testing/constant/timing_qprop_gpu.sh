#!/bin/bash
#SBATCH -p GPU
#SBATCH --gpus=128
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=128
#SBATCH --ntasks-per-node=128
#SBATCH -t 01:00:00
#SBATCH -N 1
#SBATCH -o testing/constant/output_timing_qprop_gpu_%j.out

module purge
module load gcc/13.2.1-p20240113
module load cuda
module load nvhpc

cd ~/parallel-seg-trees/code

nvidia-smi

# make clean
# make

# # Cuda Prefix
# ./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m cudaprefix -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m cudaprefix -i 5 -v

# # Cuda Levels
# ./rangequery -f ./inputs/milestone/qprop_milestone_a_n_262144_o_262144_c_2048_r_20_q_0.10.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_b_n_262144_o_262144_c_2048_r_20_q_0.20.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_c_n_262144_o_262144_c_2048_r_20_q_0.30.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_d_n_262144_o_262144_c_2048_r_20_q_0.40.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_e_n_262144_o_262144_c_2048_r_20_q_0.50.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_f_n_262144_o_262144_c_2048_r_20_q_0.60.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_g_n_262144_o_262144_c_2048_r_20_q_0.70.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_h_n_262144_o_262144_c_2048_r_20_q_0.80.txt -m cudalevels -i 5 -v
# ./rangequery -f ./inputs/milestone/qprop_milestone_i_n_262144_o_262144_c_2048_r_20_q_0.90.txt -m cudalevels -i 5 -v