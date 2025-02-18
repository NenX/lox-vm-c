#include <stdarg.h>
#include "debug.h"
#include "vm.h"
#include "compiler.h"
#include <stdio.h>
VM vm;

static void resetStack()
{
    vm.stackTop = vm.stack;
}
static void runtimeError(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
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
static Value peek(int distance)
{
    return vm.stackTop[-1 - distance];
}
static bool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
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
#ifdef DEBUG_TRACE_EXECUTION
    printf("== run ==");
#endif

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op)                        \
    do                                                  \
    {                                                   \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) \
        {                                               \
            runtimeError("Operands must be numbers.");  \
            return INTERPRET_RUNTIME_ERROR;             \
        }                                               \
        double b = AS_NUMBER(pop());                    \
        double a = AS_NUMBER(pop());                    \
        push(valueType(a op b));                        \
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
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        case OP_EQUAL:
        {
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(valuesEqual(a, b)));
            break;
        }
        case OP_GREATER:
            BINARY_OP(BOOL_VAL, >);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL, <);
            break;
        case OP_ADD:
            BINARY_OP(NUMBER_VAL, +);
            break;
        case OP_SUBTRACT:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            BINARY_OP(NUMBER_VAL, /);
            break;
        case OP_NOT:
            push(BOOL_VAL(isFalsey(pop())));
            break;
        case OP_NEGATE:
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_RETURN:
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
    }

#ifdef DEBUG_TRACE_EXECUTION
    printf("== run ==");
#endif

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

// 解释字节码，并返回解释结果
InterpretResult interpret(const char *source)
{
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk))
    {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}
