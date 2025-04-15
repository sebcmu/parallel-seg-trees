#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <vector>
#include <atomic>

int parent(int index);
int leftChild(int index);
int rightChild(int index);
int computeSum(int u, int i, int j, int L, int R, const std::vector<int>& ST);
int lockFreeComputeSum(int u, int i, int j, int L, int R, std::atomic<int>* ST);

#endif