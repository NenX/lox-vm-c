#include "debug.h"
#include "vm.h"

#include <stdio.h>
VM vm;

static void resetStack()
{
    vm.stackTop = vm.stack;
}

void initVM()
{
    resetStack();
}

void freeVM()
{
}

// 从栈顶弹出一个值，并返回该值
Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}
// 将一个值压入栈顶
void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

// 运行字节码
static InterpretResult run()
{

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)     \
    do                    \
    {                     \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

    // 循环执行字节码
    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        // 任何指令的第一个字节都是操作码
        // 给定一个操作码，执行对应的操作，这叫做解码(decoding)或者指令分配(dispatching the instruction)
        // 这里是整个虚拟机性能最关键的部分(the most performance-critical part of the VM)
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        case OP_ADD:
            BINARY_OP(+);
            break;
        case OP_SUBTRACT:
            BINARY_OP(-);
            break;
        case OP_MULTIPLY:
            BINARY_OP(*);
            break;
        case OP_DIVIDE:
            BINARY_OP(/);
            break;

        case OP_NEGATE:
            push(-pop());
            break;
        case OP_RETURN:
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

// 解释字节码，并返回解释结果
InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}
