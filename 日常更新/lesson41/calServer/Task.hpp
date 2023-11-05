#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include "log.hpp"


class Task
{
public:
    using callback_t = std::function<void (int, std::string, uint16_t)>;
private:
    int sock_; //给客户提供IO服务的sock
    uint16_t port_; //client port
    std::string ip_; //client ip
    callback_t func_; //回调方法

public:
    Task()
        :sock_(-1)
        ,port_(-1)
        {}
        //func代表回调方法，比如英汉互译， 计算器等函数。
    Task(int sock, std::string ip, uint16_t port, callback_t func)
        :sock_(sock)
        ,ip_(ip)
        ,port_(port)
        ,func_(func)
        {}
    //t()
    void operator() ()
    {
        logMessage(DEBUG, "线程ID[%p]处理%s:%d的请求 开始啦...",\
        pthread_self(), ip_.c_str(), port_);

        func_(sock_, ip_, port_);//这里实际是把func替换为传递过来的函数

        logMessage(DEBUG, "线程ID[%p]处理%s:%d的请求 结束啦...",\
            pthread_self(), ip_.c_str(), port_);

    }
    ~Task()
    {}

};