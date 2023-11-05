#pragma once

#include <iostream>
#include <string>

//自己定制协议


//定制 请求 x_ op y_

class Request
{
public:
    Request()
    {

    }
    ~Request()
    {

    }
    //序列化
    void serialize(std::string* out)
    {

    }
    //反序列化
    void deserialize(const std::string& in)
    {

    }
    //encode, 整个序列化之后的字符串进行 添加 长度
    std::string encode(const std::string& in, uint32_t len)
    {

    }

    //decode, 整个序列化之后的字符串进行 提取 长度
    std::string decode(const std::string& in, uint32_t* len)
    {

    }


private:
    //需要计算的数据
    int x_;
    int y_;

    char op_; //操作符
};


//定制响应
class Response
{
public:
    Response()
    {

    }
    ~Response()
    {

    }
    //序列化
    void serialize(std::string& out)
    {

    }
    //反序列化
    void deserialize(std::string& in)
    {

    }

    //encode 整个序列化之后的字符串添加长度
    std::string encode(const std::string& in, uint32_t len)
    {

    }

    //decode，整个序列化之后的字符串进行提取长度
    std::string decode(const std::string &in, uint32_t *len)
    {

    }


private:
     // 退出状态，0标识运算结果合法，非0标识运行结果是非法的，!0是几就表示是什么原因错了！
    int exitCode_;
    // 运算结果
    int result_;

};