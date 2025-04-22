#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <functional>
using IntCombine = std::function<int(int,int)>;

const int UPDATE = 0;
const int QUERY  = 1;

const int OPERATION_INDEX = 0;
const int QUERY_ANS = 1;

const int COMBINE_SUM_FLAG = 0;
const int COMBINE_MAX_FLAG = 1;
const int COMBINE_MIN_FLAG = 2;

#endif