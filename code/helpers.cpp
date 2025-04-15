#include <atomic>
#include "helpers.hpp"

int parent(int index){
    return (index - 1)/2;
}
int leftChild(int index){
    return 2*index + 1;
}
int rightChild(int index){
    return 2*index + 2;
}
int computeSum(int u, int i, int j, int L, int R, const std::vector<int>& ST){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return computeSum(rightChild(u),i,j,mid,R,ST);
        }
        else if (j <= mid){
            return computeSum(leftChild(u),i,j,L,mid,ST);
        }
        else{
            return computeSum(leftChild(u),i,j,L,mid,ST) + computeSum(rightChild(u),i,j,mid,R,ST);
        }
    }
}
int lockFreeComputeSum(int u, int i, int j, int L, int R, std::atomic<int>* ST){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreeComputeSum(rightChild(u),i,j,mid,R,ST);
        }
        else if (j <= mid){
            return lockFreeComputeSum(leftChild(u),i,j,L,mid,ST);
        }
        else{
            return lockFreeComputeSum(leftChild(u),i,j,L,mid,ST) + lockFreeComputeSum(rightChild(u),i,j,mid,R,ST);
        }
    }
}