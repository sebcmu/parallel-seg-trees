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
    if (non_const_flag == 1){
        return computeSumCombineNonConst(u,i,j,L,R,ST,combine_fn);
    }else{
        return computeSumCombineConst(u,i,j,L,R,ST,combine_fn);
    }
}

int computeSumCombineConst(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return computeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return computeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(computeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn),computeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int computeSumCombineNonConst(int u, int i, int j, int L, int R, const std::vector<int>& ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return computeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return computeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            current_level = (int)std::log2(u + 1);
            return combine_fn(computeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn),computeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int lockFreeComputeSumCombine(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn){
    if (non_const_flag == 1){
        return lockFreeComputeSumCombineNonConst(u,i,j,L,R,ST,combine_fn);
    }else{
        return lockFreeComputeSumCombineConst(u,i,j,L,R,ST,combine_fn);
    }
}

int lockFreeComputeSumCombineNonConst(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreeComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreeComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            current_level = (int)std::log2(u + 1);
            return combine_fn(lockFreeComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreeComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int lockFreeComputeSumCombineConst(int u, int i, int j, int L, int R, std::atomic<int>* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u];
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreeComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreeComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(lockFreeComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreeComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int finePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn){
    if (non_const_flag == 1){
        return finePaddedComputeSumCombineNonConst(u,i,j,L,R,ST,combine_fn);
    }else{
        return finePaddedComputeSumCombineConst(u,i,j,L,R,ST,combine_fn);
    }
}

int finePaddedComputeSumCombineConst(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return finePaddedComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return finePaddedComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(finePaddedComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn),finePaddedComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int finePaddedComputeSumCombineNonConst(int u, int i, int j, int L, int R, PaddedInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return finePaddedComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return finePaddedComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            current_level = (int)std::log2(u + 1);
            return combine_fn(finePaddedComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn),finePaddedComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int lockFreePaddedComputeSumCombine(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn){
    if (non_const_flag == 1){
        return lockFreePaddedComputeSumCombineNonConst(u,i,j,L,R,ST,combine_fn);
    }else{
        return lockFreePaddedComputeSumCombineConst(u,i,j,L,R,ST,combine_fn);
    }
}

int lockFreePaddedComputeSumCombineConst(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreePaddedComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreePaddedComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            return combine_fn(lockFreePaddedComputeSumCombineConst(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreePaddedComputeSumCombineConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}

int lockFreePaddedComputeSumCombineNonConst(int u, int i, int j, int L, int R, PaddedAtomicInt* ST, IntCombine combine_fn){
    if (i <= L && R <= j){
        return ST[u].value;
    }
    else {
        int mid = (L + R)/2;
        if (i >= mid){
            return lockFreePaddedComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn);
        }
        else if (j <= mid){
            return lockFreePaddedComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn);
        }
        else{
            current_level = (int)std::log2(u + 1);
            return combine_fn(lockFreePaddedComputeSumCombineNonConst(leftChild(u),i,j,L,mid,ST,combine_fn),lockFreePaddedComputeSumCombineNonConst(rightChild(u),i,j,mid,R,ST,combine_fn));
        }
    }
}
