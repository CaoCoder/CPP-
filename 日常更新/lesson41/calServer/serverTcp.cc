#include "Protocol.hpp"
#include "util.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"
#include "daemonize.hpp"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <cerrno>


#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
class ServerTcp;

/*
    数据流格式：
    1.客户端在 请求端 输入 1 + 1的值，这时候需要先进行  req.serialize序列化，然后对字符串先进encode添加报头。这样就成为了能在网络中传输的字符串
    2.服务端 拿到这个完整的报文，先去decode掉报头，得到序列化的字符串
    3.服务端再把字符串  req.deserialize反序列化 转化为 结构化的数据。然后进行计算得到的值result_,exitcode_给 响应端的对象。
    4.服务端再把响应端的结果 对象进行 resp.serialize序列化 为字符串，再encode添加报头，write回客户端。
    5.客户端 拿到数据 进行decode去掉报头，得到序列化的字符串 
    6.客户端 resp.deserialize再反序列化得到 结构体的数据 进行打印

    总结：
    1.req.serialize 
        encode
            decode
                req.deserialize
                结构化结果赋值给resp
                resp.serialize
            encode
        decode
    resp.deserialize


    req.serialize //客户端序列化
    encode          //加报头   传输给服务端
    decode          //服务端解报头
    req.deserialize// 服务端反序列化     拿到真正数据

                   计算！！ 结构化结果赋值给resp

    resp.serialize// 服务端序列化       
    encode        //加报头     传输给客户端
    decode          //客户端解报头
    resp.deserialize//客户端反序列化     拿到真正数据
        
*/
static Response calculator(const Request &req)
{
    Response resp;
    switch (req.op_)
    {
    case '+':
        resp.result_ = req.x_ + req.y_;
        break;
    case '-':
        resp.result_ = req.x_ - req.y_;
        break;
    case '*':
        resp.result_ = req.x_ * req.y_;
        break;
    case '/':
        { // x_ / y_
            if (req.y_ == 0) resp.exitCode_ = -1; // -1. 除0
            else resp.result_ = req.x_ / req.y_;
        }
    break;
    case '%':
        { // x_ / y_
            if (req.y_ == 0) resp.exitCode_ = -2; // -2. 模0
            else resp.result_ = req.x_ % req.y_;
        }
    break;
    default:
        resp.exitCode_ = -3; // -3: 非法操作符
        break;
    }

    return resp;
} 

// 1. 全部手写
// 2. 部分采用别人的方案--序列化和反序列化的问题
void netCal(int sock, const std::string &clientIp, uint16_t clientPort)
{
    assert(sock >= 0);
    assert(!clientIp.empty());
    assert(clientPort >= 1024);

    // 9\r\n100 + 200\r\n    9\r\n112 / 200\r\n
    std::string inbuffer;
    while (true)
    {
        Request req;
        char buff[128];
        //从传过来的servicesock中读取数据
        ssize_t s = read(sock, buff, sizeof(buff) - 1);
        if (s == 0)
        {
            logMessage(NOTICE, "client[%s:%d] close sock, service done", clientIp.c_str(), clientPort);
            break;
        }
        else if (s < 0)
        {
            logMessage(WARINING, "read client[%s:%d] error, errorcode: %d, errormessage: %s",
                       clientIp.c_str(), clientPort, errno, strerror(errno));
            break;
        }
        //走到这里说明 s > 0
        // read success
        buff[s] = 0;
        inbuffer += buff;//这时候就是一个带有报头的字符串
        std::cout << "inbuffer: " << inbuffer << std::endl;
        // 1. 检查inbuffer是不是已经具有了一个strPackage
        uint32_t packageLen = 0;
        std::string package = decode(inbuffer, &packageLen); //TODO
        if (packageLen == 0) continue; // 无法提取一个完整的报文，继续努力读取吧

        std::cout << "package: " << package << std::endl;
        // 2. 已经获得一个完整的package
        if (req.deserialize(package))//字符串变为 结构化数据
        {
            req.debug();
            // 3. 处理逻辑, 输入的是一个req，得到一个resp
            Response resp = calculator(req); //resp是一个结构化的数据
            // 4. 对resp进行序列化
            std::string respPackage;
            resp.serialize(&respPackage);
            cout << "结果" << respPackage << endl;
            // 5. 对报文进行encode -- //TODO
            respPackage = encode(respPackage, respPackage.size());
            // 6. 简单进行发送 -- 后续处理
            write(sock, respPackage.c_str(), respPackage.size());
        }
    }
}


class ServerTcp
{
public:
    ServerTcp(uint16_t port, const std::string& ip = "")
        :port_(port)
        ,ip_(ip)
        ,listenSock_(-1)
        ,tp_(nullptr)
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
            logMessage(FATAL, "SOCKET: %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        logMessage(DEBUG, "socket: %s, %d", strerror(errno), listenSock_);

        //2.bind绑定
        //2.1填充服务器信息
        struct sockaddr_in local;//用户栈
        memset(&local, 0, sizeof(local));
        local.sin_family =PF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));

        //2.2本地socket信息写入到sock_对应的内核区域
        if(bind(listenSock_, (const struct sockaddr*)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }
        logMessage(DEBUG, "bind: %s, %d", strerror(errno), listenSock_);  

        //3.监听socket,因为tcp是面向连接的
        if(listen(listenSock_, 5) < 0)
        {
            logMessage(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);

        }
        logMessage(DEBUG, "listen: %s, %d", strerror(errno), listenSock_);
        // 运行别人来连接你了

        //4.加载线程池
        tp_ = ThreadPool<Task>::getInstance();


    }

    void loop()
    {
        tp_->start();//启动线程池,创建5个线程,线程同时执行例程函数，然后拿上锁，在条件变量下进行等待。
        logMessage(DEBUG, "thread pool start success, thread num: %d", tp_->threadNum());

        while(!quit_)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            //4.获取连接
            //4.1 listenSock_: 监听 && 获取新的链接-> sock
            //4.2 serviceSock: 给用户提供新的socket服务
            int serviceSock = accept(listenSock_, (struct sockaddr* )&peer, &len);
            if(quit_)
                break;
            if(serviceSock < 0)
            {
                //获取链接失败
                 logMessage(WARINING, "accept: %s[%d]", strerror(errno), serviceSock);
                continue;
           
            }

            //4.1获取客户端的基本信息
            uint16_t peerPort = ntohs(peer.sin_port);
            std::string peerIp = inet_ntoa(peer.sin_addr);
            logMessage(DEBUG, "accept: %s | %s[%d], socket fd: %d",
                       strerror(errno), peerIp.c_str(), peerPort, serviceSock);
            //5.提供服务

            Task t(serviceSock, peerIp, peerPort, netCal);
            tp_->push(t);


        }

    }
    bool quitServer()
    {
        quit_ = true;
        return true;
    }
private:
    int listenSock_;
    uint16_t port_;
    std::string ip_;

    ThreadPool<Task>* tp_;//引入线程池

    bool quit_;//安全退出

};









static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
    std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n"
              << std::endl;
}

ServerTcp* svrp = nullptr;
void sigHandler(int signo)
{
    if(signo == 3 && svrp != nullptr)
    {
        svrp->quitServer();

    }
    logMessage(DEBUG, "server quit save!");
}
int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::string ip;
    if(argc == 3)
        ip = argv[2];
    //注册信号处理函数 sigHandler，将信号编号为 3（SIGQUIT 信号）与该处理函数关联。
    signal(3, sigHandler);
    ServerTcp svr(port, ip);
    svr.init();
    svrp = &svr;
    svr.loop();
    return 0;
}
