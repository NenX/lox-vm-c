#include <stdio.h>
#include "value.h"
#include "debug.h"
// 静态函数只在本文件中可见
// 简单指令，打印指令名称，并返回下一个指令的偏移量
static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}
// 打印指令名称，并打印常量值。返回下一个指令的偏移量
static int constantInstruction(const char *name, Chunk *chunk, int offset)
{
    // offset 是当前指令的偏移量
    // offset + 1 是常量表索引的偏移量
    // constant 是常量表索引
    uint8_t constant = chunk->code[offset + 1];
    // 打印指令名称，%-16s 表示指令名称最少16个字符宽，%4d 表示常量值最少4个字符宽
    printf("%-16s %4d '", name, constant);
    // 打印常量值
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    // 返回下一个指令的偏移量
    return offset + 2;
}

// 反汇编chunk，并打印字节码和行号
void disassembleChunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    // 遍历所有字节码
    for (int offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
    printf("== %s ==\n", name);
}
// 反汇编指令
int disassembleInstruction(Chunk *chunk, int offset)
{
    printf("%04d ", offset);

    // 如果当前行号与前一行号相同，则打印“|”，否则打印行号
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ", chunk->lines[offset]);
    }

    // 获取当前字节码
    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_NIL:
        return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
        return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
    case OP_EQUAL:
        return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
        return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
        return simpleInstruction("OP_LESS", offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:
        return simpleInstruction("OP_NOT", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
