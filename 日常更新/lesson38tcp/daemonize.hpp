#pragma once

#include <cstdio>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void daemonize()
{
    int fd = 0;
    //忽略掉sigpipe信号
    signal(SIGPIPE, SIG_IGN);
    //让自己不要成为进程组组长
    if(fork() > 0)//父进程
    {
        exit(1);
    }
    //设置自己是一个独立的会话
    setsid();
    //重定向0 1 2到垃圾桶
    if((fd = open("/dev/null", O_RDWR)) != -1)
    {
        dup2(fd, STDIN_FILENO);//用fd覆盖标准输入STDIN_FILENO
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        //关闭不需要的fd
        if(fd > STDERR_FILENO)
            close(fd);

    }
}