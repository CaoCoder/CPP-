#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <string.h>
#include "util.hpp"
/*
头文件提供了代码的声明部分，帮助编译器正确解析代码，
而库文件提供了代码的实际定义部分，
并需要在编译时通过 -l 选项告知链接器进行链接。
*/
#include <jsoncpp/json/json.h>


#define CRLF "\r\n"
#define CRLF_LEN strlen(CRLF)
#define SPACE " "
#define SPACE_lEN strlen(SPACE)


#define OPS "+-*/%"
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
    //序列化 把 结构化数据 -> 字符串
    void serialize(std::string* out)
    {
#ifdef MY_SELF
        std::string xstr = std::to_string(x_);
        std::string ystr = std::to_string(y_);

        *out = xstr;
        *out += SPACE;
        *out += op_;
        *out += SPACE;
        *out += ystr;
#else
    //json
    //1.Value对象，万能对象
    //2.json是基于KV
    //4.有两套操作方法。序列化的时候，会将所有的数据内容转化为字符串
    Json::Value root;
    root["x"] = x_;
    root["y"] = y_;
    root["op"] = op_;

    // Json::FastWriter fw;
    Json::StyledWriter fw;
    //这里使用的是 Json::FastWriter，它会生成紧凑的 JSON 字符串，
    //可能会牺牲一些可读性和格式化。如果需要更具可读性的输出，
    //可以使用 Json::StyledWriter 或 Json::StreamWriter 等其他写入器类。
    *out = fw.write(root);
#endif

    }
    //反序列化 字符串 -> 结构化的数据
    bool deserialize(std::string& in)
    {
#ifdef MY_SELF
        //100 + 200
        std::size_t spaceOne = in.find(SPACE);//从前往后找空格
        if(std::string::npos == spaceOne)
            return false;
        std::size_t spaceTwo = in.rfind(SPACE);//从后往前找空格
        if(std::string::npos == spaceTwo)
            return false;
        
        std::string dataOne = in.substr(0, spaceOne);
        std::string dataTwo = in.substr(spaceTwo + SPACE_lEN);
        std::string oper = in.substr(spaceOne + SPACE_lEN, spaceTwo - (spaceOne + SPACE_lEN));

        if(oper.size() != 1)
            return false;
        
        //转成内部成员
        x_ = atoi(dataOne.c_str());
        y_ = atoi(dataTwo.c_str());
        op_ = oper[0];

        return true;
#else

        Json::Value root;
        Json::Reader rd;
        rd.parse(in, root);//读取并将解析结果存储在 root 对象中
        x_ = root["x"].asInt();
        y_ = root["y"].asInt();
        op_ = root["op"].asInt();
        return true;
#endif
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
#ifdef MY_SELF
        //"exitCode_ result_"
        std::string ec = std::to_string(exitCode_);
        std::string res = std::to_string(result_);

        *out = ec;
        *out += SPACE;
        *out += res;
#else

        Json::Value root;
        root["exitcode"] = exitCode_;
        root["result"] = result_;
        Json::FastWriter fw;
        *out = fw.write(root);
#endif
    }
    //反序列化 将反序列化后的数据写入到内部成员，形成结构化数据
    bool deserialize(std::string& in)
    {
#ifdef MY_SELF
        // "0 100" 状态码 结果
        std::size_t pos = in.find(SPACE);
        if(std::string::npos == pos)
            return false;
        std::string codestr = in.substr(0, pos);//提取状态码
        std::string reststr = in.substr(pos + SPACE_lEN);//跳过空格提取结果

        //将反序列化后的数据写入到内部成员，形成结构化数据
        exitCode_ = atoi(codestr.c_str());
        result_ = atoi(reststr.c_str());

        return true;
#else
        Json::Value root;
        Json::Reader rd;
        rd.parse(in, root);
        exitCode_ = root["exitcode"].asInt();
        result_ = root["result"].asInt();
        return true;
#endif
    }



public:
     // 退出状态，0标识运算结果合法，非0标识运行结果是非法的，!0是几就表示是什么原因错了！
    int exitCode_;
    // 运算结果
    int result_;

};
#define OPS "+-*/%"
bool makeRequest(const std::string& str, Request* req)
{
        // 123+1  1*1 1/1
    char strtmp[BUFFER_SIZE];
    snprintf(strtmp, sizeof strtmp, "%s", str.c_str());
    char *left = strtok(strtmp, OPS);
    if (!left)
        return false;
    char *right = strtok(nullptr, OPS);
    if (!right)
        return false;
    char mid = str[strlen(left)];

    req->x_ = atoi(left);
    req->y_ = atoi(right);
    req->op_ = mid;
    return true;

}