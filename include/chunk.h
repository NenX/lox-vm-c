#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
// OpCode 枚举类型，包含所有可能的指令
typedef enum
{
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_RETURN,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE
} OpCode;

// Chunk 字节码结构体
typedef struct
{
    int count;            // 字节码数量
    int capacity;         // 字节码容量
    uint8_t *code;        // 字节码数组
    int *lines;           // 行号数组
    ValueArray constants; // 常量表
} Chunk;

// 初始化chunk
void initChunk(Chunk *chunk);
// 向chunk中写入字节码
void writeChunk(Chunk *chunk, uint8_t byte, int line);
// 释放chunk
void freeChunk(Chunk *chunk);
// 向chunk的常量表中添加常量，并返回常量在常量表中的索引
int addConstant(Chunk *chunk, Value value);

#endif
