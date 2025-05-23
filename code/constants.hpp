#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cmath>
#include <functional>
#include <mutex>
#include <new>
#include <atomic>
#include <thread>
#include <chrono>

extern int num_levels;
extern thread_local int current_level;
extern int non_const_flag;

using IntCombine = std::function<int(int,int)>;
const size_t CACHE_LINE_SIZE = 64;

struct alignas(CACHE_LINE_SIZE) PaddedAtomicInt{
    std::atomic<int> value;
};
struct alignas(CACHE_LINE_SIZE) PaddedInt{
    int value;
};
struct alignas(CACHE_LINE_SIZE) PaddedMutex{
    std::mutex m;
};

const int UPDATE = 0;
const int QUERY  = 1;

const int OPERATION_INDEX = 0;
const int QUERY_ANS = 1;

const int COMBINE_SUM_FLAG = 0;
const int COMBINE_MAX_FLAG = 1;
const int COMBINE_MIN_FLAG = 2;

const int CONTIGUOUS_UPDATES_PER_THREAD = 8;

#endif
