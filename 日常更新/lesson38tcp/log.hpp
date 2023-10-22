#pragma once

#include <cstdio>
#include <ctime>
#include <assert.h>
#include <cstdlib>
#include <stdarg.h>

#define DEBUG 0
#define NOTICE 1
#define WARINING 2
#define FATAL 3

const char* log_level[] = {"DEBUG", "NOTICE", "WARINING", "FATAL"};
void logMessage(int level, const char* format, ...)
{
    assert(level >= DEBUG);
    assert(level <= FATAL);

    char* name = getenv("USER");//获得一个环境变量

    char logInfo[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(logInfo, sizeof(logInfo)-1, format, ap);


    va_end(ap);
    //判断是写到标准错误还是标准输出
    FILE* out = (level == FATAL) ? stderr : stdout;
    fprintf(out, "%s | %u | %s | %s\n", \
        log_level[level], \
        (unsigned int)time(nullptr), \
        name == nullptr ? "unknow" : name, \
        logInfo);

    
}