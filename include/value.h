#ifndef clox_value_h
#define clox_value_h

#include "common.h"

// 值类型
typedef double Value;

// 值数组
typedef struct
{
    int capacity;  // the maximum number of values that can be stored in the array
    int count;     // the number of values in the array
    Value *values; // the array of values
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif
