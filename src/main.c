#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "common.h"
#include "chunk.h"
#include "debug.h"

static void repl()
{
    char line[1024];
    for (;;)
    {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        interpret(line);
    }
}
// 读取文件，返回文件内容指针
static char *readFile(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    // 将文件指针移动到文件末尾
    fseek(file, 0L, SEEK_END);
    // 获取文件大小
    size_t fileSize = ftell(file);
    // 将文件指针移动到文件开头
    rewind(file);
    // 分配内存，分配fileSize + 1 个字节，因为需要存储文件结束符
    char *buffer = (char *)malloc(fileSize + 1);
    // 读取文件内容
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    // 如果读取的文件大小小于文件大小，则报错
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    // 设置文件结束符
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}
// 运行文件
static void runFile(const char *path)
{
    char *source = readFile(path);

    // 解释器运行
    InterpretResult result = interpret(source);

    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}

// Entry point
int main(int argc, const char *argv[])
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    initVM();
    // 如果命令行参数为空，则进入 repl 模式
    if (argc == 1)
    {
        repl();
    }
    // 如果有一个参数，则运行文件
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    freeVM();

    return 0;
}
