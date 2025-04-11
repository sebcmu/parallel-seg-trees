#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <vector>

int parent(int index);
int leftChild(int index);
int rightChild(int index);
int computeSum(int u, int i, int j, int L, int R, const std::vector<int>& ST);

#endif