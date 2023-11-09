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

        // read success
        buff[s] = 0;
        inbuffer += buff;
        // 1. 检查inbuffer是不是已经具有了一个strPackage
        uint32_t packageLen = 0;
        std::string package = decode(inbuffer, &packageLen); //TODO
        if (packageLen == 0) continue; // 无法提取一个完整的报文，继续努力读取吧
        // 2. 已经获得一个完整的package
        if (req.deserialize(package))
        {
            req.debug();
            // 3. 处理逻辑, 输入的是一个req，得到一个resp
            Response resp = calculator(req); //resp是一个结构化的数据
            // 4. 对resp进行序列化
            std::string respPackage;
            resp.serialize(&respPackage);
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
        logMessage(DEBUG, "socket: %s, %d", strerror(errno));

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
    
    signal(3, sigHandler);
    ServerTcp svr(port, ip);
    svr.init();
    svrp = &svr;
    svr.loop();
    return 0;
}
