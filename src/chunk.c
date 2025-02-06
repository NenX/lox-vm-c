#include "chunk.h"
#include "memory.h"
#include <stdlib.h>

void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;

    initValueArray(&chunk->constants);
}
// 向 chunk->code 中写入字节码，并增加 chunk->count    
void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void freeChunk(Chunk *chunk)
{
    // 释放字节码   
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);

    // 释放行号
    FREE_ARRAY(int, chunk->lines, chunk->capacity);

    // 释放常量表
    freeValueArray(&chunk->constants);
    // 初始化chunk
    initChunk(chunk);
}
// 向chunk的常量表中添加常量，并返回常量在常量表中的索引
int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value); // chunk->constants.count 增加1
    return chunk->constants.count - 1; // 返回常量在常量表中的索引
}