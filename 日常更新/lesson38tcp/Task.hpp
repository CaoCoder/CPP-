#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include "log.hpp"


class Task
{
public:
    //声明回调方法
     typedef std::function<void (int, std::string, uint16_t)> callback_t;
private:
    int sock_;//给用户提供IO服务的sock
    uint16_t port_; //客户端 端口号
    std::string ip_;//客户端 ip
    callback_t func_;//回调方法
public:
    Task()
        :sock_(-1)
        ,port_(-1)
        {};
    Task(int sock, std::string ip, uint16_t port, callback_t func)
        :sock_(sock)
        ,ip_(ip)
        ,port_(port)
        ,func_(func)
    {};

    void operator() ()
    {
        logMessage(DEBUG, "线程ID[%P]处理%s:%d的请求开始啦...",
        pthread_self(), ip_.c_str(), port_);

        func_(sock_, ip_, port_);

        logMessage(DEBUG, "线程ID[%p]处理%s:%d的请求 结束啦...",\
        pthread_self(), ip_.c_str(), port_);

    }
};