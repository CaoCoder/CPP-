#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <iostream>
#include <functional>
#include <string>
// ����
int add(int a, int b) {
    cout << "��ð���" << endl;
    return a + b;

}

// ��������
struct Multiply {
    int operator()(int a, int b) {
        cout << "��ò��ã�" << endl;

        return a * b;
    }
};
using sb = char;

//typedef std::function<int(int, int)> callback_t;
using callback_t = std::function<int(int, int)>;
void performOperation(int num, std::string message, uint16_t value, callback_t callback) {
    // ִ��ĳЩ����...

    // ���ûص�����
    callback(2,2);
}

void myCallback(int num, std::string message, uint16_t value) {
    std::cout << "Callback received: " << num << ", " << message << ", " << value << std::endl;
}

int main() {
    auto b = [](int a, int b) {
        cout << "����ô�ˣ�" << endl;
        return a + b;
    };
    Multiply m;
    // �����ص���������
    callback_t myCallbackVar = b;
    std::function<int(int, int)> func1 = b;
    // ���ú����������ݻص�������Ϊ����
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
        //���Ե�sigpipe�ź�
        signal(SIGPIPE, SIG_IGN);
        //���Լ���Ҫ��Ϊ�������鳤
        if (fork() > 0)//������
        {
            exit(1);
        }
        //�����Լ���һ�������ĻỰ
        setsid();
        //�ض���0 1 2������Ͱ
        if ((fd = open("/dev/null", O_RDWR)) != -1)
        {
            dup2(fd, STDIN_FILENO);//��fd���Ǳ�׼����STDIN_FILENO
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);

            //�رղ���Ҫ��fd
            if (fd > STDERR_FILENO)
                close(fd);

        }
    }
};