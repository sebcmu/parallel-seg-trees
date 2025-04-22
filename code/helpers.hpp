#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <vector>
#include <atomic>
#include "constants.hpp"

int parent(int index);
int leftChild(int index);
int rightChild(int index);
int computeSum(int u, int i, int j, int L, int R, const std::vector<int>& ST);
int lockFreeComputeSum(int u, int i, int j, int L, int R, std::atomic<int>* ST);
int computeSumCombine(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn);
int lockFreeComputeSumCombine(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn);

#endif