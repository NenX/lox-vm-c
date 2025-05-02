#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "common.h"
#include "object.h"
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
    uint8_t *ip; // 当前执行的指令, 指向 vm.chunk->code
    Value stack[STACK_MAX]; // 运行时储存值的栈   
    Value *stackTop; // 栈顶指针
    Obj *objects; // 指向单向链表的头节点
} VM;
// extern 用于声明一个变量，该变量在其他文件中定义
extern VM vm;
void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
#endif

