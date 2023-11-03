#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <iostream>
#include <functional>
#include <string>
// 函数
int add(int a, int b) {
    cout << "你好啊！" << endl;
    return a + b;

}

// 函数对象
struct Multiply {
    int operator()(int a, int b) {
        cout << "你好不好？" << endl;

        return a * b;
    }
};
using sb = char;

//typedef std::function<int(int, int)> callback_t;
using callback_t = std::function<int(int, int)>;
void performOperation(int num, std::string message, uint16_t value, callback_t callback) {
    // 执行某些操作...

    // 调用回调函数
    callback(2,2);
}

void myCallback(int num, std::string message, uint16_t value) {
    std::cout << "Callback received: " << num << ", " << message << ", " << value << std::endl;
}

int main() {
    auto b = [](int a, int b) {
        cout << "你怎么了？" << endl;
        return a + b;
    };
    Multiply m;
    // 声明回调函数变量
    callback_t myCallbackVar = b;
    std::function<int(int, int)> func1 = b;
    // 调用函数，并传递回调函数作为参数
    performOperation(42, "Hello", 123, myCallbackVar);
    sb sb = 'a';
    
    cout << sb;

    return 0;
}

namespace csq
{
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
        if (fork() > 0)//父进程
        {
            exit(1);
        }
        //设置自己是一个独立的会话
        setsid();
        //重定向0 1 2到垃圾桶
        if ((fd = open("/dev/null", O_RDWR)) != -1)
        {
            dup2(fd, STDIN_FILENO);//用fd覆盖标准输入STDIN_FILENO
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);

            //关闭不需要的fd
            if (fd > STDERR_FILENO)
                close(fd);

        }
    }
};