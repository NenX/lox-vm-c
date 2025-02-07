#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "common.h"
#include "value.h"
#define STACK_MAX 256

// 解释结果
typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

// 虚拟机结构体
typedef struct
{
    Chunk *chunk; // 字节码 
    uint8_t *ip; // 当前执行的指令
    Value stack[STACK_MAX]; // 储存值的栈   
    Value *stackTop; // 栈顶指针
} VM;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
#endif
