#pragma once

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cassert>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const char *log_level[] = {"DEBUG", "NOTICE", "WARINING", "FATAL"};
#define LOGFILE "serverTcp.log"

#define DEBUG 0
#define NOTICE 1
#define WARINING 2
#define FATAL 3

class Log
{
public:
    Log()
        :logFd(-1)
        {}
    
    void enable()
    {
        umask(0);
        logFd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
        assert(logFd != -1);
        dup2(logFd, 1);
        dup2(logFd, 2);
    }

    ~Log()
    {
        if(logFd != -1)
        {
            fsync(logFd);
            close(logFd);
        }
    }
private:
    int logFd;
};

//format是要格式化的字符串的格式 ,  ... 三个点是传入的可变参数。
void logMessage(int level, const char* format, ...)
{
    assert(level >= DEBUG);
    assert(level <= FATAL);
    char* name = getenv("USER");

    char logInfo[1024];
    //这四行代码对格式进行处理 存储到logInfo中。
    va_list ap;
    va_start(ap, format);
    vsnprintf(logInfo, sizeof(logInfo)-1, format, ap);
    va_end(ap);

    FILE* out = (level == FATAL) ? stderr : stdout;
    fprintf(out, "%s | %u | %s | %s\n",
            log_level[level],
            (unsigned int)time(nullptr),
            name == nullptr ? "unknow" : name,
            logInfo);
    
    fflush(out); //将C语言缓冲区的数据刷新到0S
    fsync(fileno(out)); //将OS中的数据尽快刷盘
    

}