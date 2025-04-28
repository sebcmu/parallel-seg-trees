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
int computeSumCombineConst(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn);
int computeSumCombineNonConst(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn);
int lockFreeComputeSumCombine(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn);
int lockFreeComputeSumCombineConst(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn);
int lockFreeComputeSumCombineNonConst(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn);
int lockFreePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn);
int lockFreePaddedComputeSumCombineConst(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn);
int lockFreePaddedComputeSumCombineNonConst(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn);
int finePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn);
int finePaddedComputeSumCombineConst(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn);
int finePaddedComputeSumCombineNonConst(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn);

#endif
