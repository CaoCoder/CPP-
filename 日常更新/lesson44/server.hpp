#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <cerrno>
#include <cassert>

#define CRLF "\r\n"
#define SPACE " "
#define SPACE_LEN strlen(SPACE)
#define HOME_PAGE "index.html"
#define ROOT_PATH "wwwroot"

using namespace std;

class ServerTcp
{
public:
    ServerTcp(uint16_t port, const std::string& ip = "")
        :port_(port)
        ,ip_(ip)
        ,listenSock_(-1)
    {
        quit_ = false;
    }

    ~ServerTcp()
    {
        if(listenSock_ >= 0)
            close(listenSock_);
    }

public:
    void init()
    {
        //1.创建socket
        listenSock_ = socket(PF_INET, SOCK_STREAM, 0);
        if(listenSock_ < 0)
        {
            exit(1);
        }
        //2.绑定
        struct sockaddr_in local;//用户栈
        local.sin_port = htons(port_);
        local.sin_family = PF_INET;
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));
        if(bind(listenSock_, (const struct sockaddr*)&local, sizeof(local)) < 0)
        {
            exit(2);
        }

        //3.监听套接字
        if(listen(listenSock_, 5) < 0)
        {
            exit(3);
        }

    }


    void loop()
    {
        signal(SIGCHLD, SIG_IGN);//发给子进程的信号动作设置为忽略
        while(!quit_)
        {
            
        }
    }
private:
    //sock
    int listenSock_;
    //port
    uint16_t port_;
    //ip
    std::string ip_;
    //安全退出
    bool quit_;
};