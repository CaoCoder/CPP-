#pragma once

#include <iostream>
#include <string>
#include <assert.h>

#define CRLF "\r\n"
#define CRLF_LEN strlen(CRLF)
#define SPACE " "
#define SPACE_lEN strlen(SPACE)

//自己定制协议

//定制 请求 x_ op y_
    //encode, 整个序列化之后的字符串进行 添加 长度
    std::string encode(const std::string& in, uint32_t len)
    {
        // "状态码 结果"
        //"len\r\n""exitCode_ result_\r\n"

        std::string encodein = std::to_string(len);//将长度转化为字符串
        encodein += CRLF;//加上/r/n
        encodein += in;//再加上状态码和结果
        encodein += CRLF;// 再加上\r\n
        return encodein;// 返回添加了长度的字符串
    }

    //decode, 整个序列化之后的字符串进行 提取 
    std::string decode(std::string& in, uint32_t* len)
    {
        assert(len);
        //1.确认是否是一个包含len的有效字符串
        *len = 0;
        std::size_t pos = in.find(CRLF);//查找是否有\r\n
        if(pos == std::string::npos)
            return "";//返回空串
        
        //2.提取长度
        std::string inLen = in.substr(0, pos);//提取长度的 字符串
        int intLen = atoi(inLen.c_str());
        //3.确认 有效载荷 也是合法的
        int surplus = in.size() - 2 * CRLF_LEN - pos;//获得
        if (surplus < intLen)
            return "";
        
        //4.确定有完整的报文结构
        std::string package = in.substr(pos + CRLF_LEN, intLen);
        *len = intLen;//赋值给字符串长度len

        //5.将当前报文完整的从in中全部移除掉

        int removeLen = inLen.size() + package.size() + 2 * CRLF_LEN;//计算整个报文的长度
        // in.erase(0, removeLen);
        in.erase(0, removeLen);
        //6.正常返回
        return package;//返回有效载荷
    }


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
    //反序列化 字符串 -> 结构化的数据
    bool deserialize(const std::string& in)
    {
        //100 + 200
        std::size_t spaceOne = in.find(SPACE);//从前往后找空格
        if(std::string::npos == spaceOne);
            return false;
        std::size_t spaceTwo = in.rfind(SPACE);//从后往前找空格
        if(std::string::npos == spaceTwo)
            return false;
        
        std::string dataOne = in.substr(0, spaceOne);
        std::string dataTwo = in.substr(spaceTwo + SPACE_lEN);
        std::string oper = in.substr(spaceOne + SPACE_lEN);

        if(oper.size() != 1)
            return false;
        
        //转成内部成员
        x_ = atoi(dataOne.c_str());
        y_ = atoi(dataTwo.c_str());
        op_ = oper[0];

        return true;

    }
    void debug()
    {
        std::cout << "#################################" << std::endl;
        std::cout << "x_: " << x_ << std::endl;
        std::cout << "op_: " << op_ << std::endl;
        std::cout << "y_: " << y_ << std::endl;
        std::cout << "#################################" << std::endl;
    }



public:
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
        :exitCode_(0)
        ,result_(0)
    {

    }
    ~Response()
    {

    }
    //序列化 把结构 变为 字符串数据
    void serialize(std::string* out)
    {
        //"exitCode_ result_"
        std::string ec = std::to_string(exitCode_);
        std::string res = std::to_string(result_);

        *out = ec;
        *out += SPACE;
        *out += res;
    }
    //反序列化
    void deserialize(std::string& in)
    {
      

    }



public:
     // 退出状态，0标识运算结果合法，非0标识运行结果是非法的，!0是几就表示是什么原因错了！
    int exitCode_;
    // 运算结果
    int result_;

};