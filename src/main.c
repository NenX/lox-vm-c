#include <stdio.h>
#include "vm.h"
#include "common.h"
#include "chunk.h"
#include "debug.h"

#include <stdlib.h> // in order to use system
// 清空控制台
void clearConsole()
{
    system("clear");
}

// Entry point
int main(void)
{
    clearConsole();
    initVM();
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2); // constant 是常量在常量表中的索引
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_ADD, 123);

    constant = addConstant(&chunk, 2.3);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_DIVIDE, 123);

    writeChunk(&chunk, OP_NEGATE, 123);
    
    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test");
    freeVM();

    interpret(&chunk);
    freeChunk(&chunk);

    return 0;
}
