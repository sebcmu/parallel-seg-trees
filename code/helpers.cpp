#include <atomic>
#include "helpers.hpp"
#include "constants.hpp"

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
int computeSumCombine(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return computeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return computeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(computeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn),computeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}
int lockFreeComputeSumCombine(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreeComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreeComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(lockFreeComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreeComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int lockFreePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreePaddedComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreePaddedComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(lockFreePaddedComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreePaddedComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int finePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return finePaddedComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return finePaddedComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(finePaddedComputeSumCombine(leftChild(u),i,j,L,mid,ST,combine_fn),finePaddedComputeSumCombine(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}