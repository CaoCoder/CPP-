#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
typedef void (*handler_t)();


//方法集合
std::vector<handler_t> handlers;

void fun1()
{
    std::cout << "你好啊，我是方法1" <<std::endl;
}
void fun2()
{
    std::cout << "你好啊，我是方法2" <<std::endl;
}
void Load()
{
    handlers.push_back(fun1);
    handlers.push_back(fun2);
}
int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //子进程
        while(1)
        {
            printf("我是子进程，我的PID：%d, 我的PPID：%d\n",getpid(),getppid());
            sleep(3);
            exit(104);
        }
        printf("执行\n");
       
        printf("完毕\n");

    }
    else if (id > 0)
    {
        //父进程 基于非阻塞的轮询等待方案
        // int status = 0;
        // while(1)
        // {
        //     pid_t ret = waitpid(-1, &status,WNOHANG);
        //     if(ret > 0)
        //     {
        //         printf("等待成功，%d,exit sig : %d, exit code:%d\n",ret,status&0x7F, (status>>8)&0xFF);
        //         break;
        //     }
        //     else if(ret == 0)
        //     {
        //         //等待成功，但是子进程没有退出
        //         printf("子进程还没退出，父进程需要做其他事情。。。\n");
        //         if(handlers.empty())
        //         {
        //             Load();
        //         }
        //         for(auto f : handlers)
        //         {
        //             f();
        //         }
        //         sleep(1);
        //     }
        //     else
        //     {
        //         //出错了暂不处理
        //     }
        // }

        printf("我是父进程, 我的PID: %d, 我的PPID:%d\n", getpid(), getppid());
        int status = 0;
        sleep(20);

        //options: 0表示阻塞等待
        pid_t ret = waitpid(-1, &status, 0);
        if(ret > 0)
        {       if(WIFEXITED(status)) 
                {
                  printf("等待成功了啊：exit code: %d\n", WEXITSTATUS(status));
                }
               printf("等待成功, %d, exit sig: %d, exit code: %d\n", ret, status&0x7F, (status>>8)&0xFF);
        }
        sleep(3);
    }
    else{
        //do nothing
    }

    

}