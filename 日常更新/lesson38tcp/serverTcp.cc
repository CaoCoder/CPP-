#include "util.hpp"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

class ServerTcp;
class ThreadData
{
public:
    uint16_t clientPort_;
    std::string clinetIp_;
    int sock_;
    ServerTcp *this_;
public:
    ThreadData(uint16_t port, std::string ip, int sock,  ServerTcp *ts)
        : clientPort_(port), clinetIp_(ip), sock_(sock),this_(ts)
    {}
};

class ServerTcp
{
public:
    ServerTcp(uint16_t port,const std::string& ip = " " )
        :port_(port)
        ,ip_(ip)
        {}
    ~ServerTcp()
    {

    }
public:
    void init()
    {
        //1.创建socket （监听套接字）
        listenSock_ = socket(AF_INET, SOCK_STREAM, 0);
        if(listenSock_  < 0)
        {
            logMessage(FATAL, "socket: %s", strerror(errno));
            exit(SOCKET_ERR);
        }

        logMessage(DEBUG, "socket: %s, %d", strerror(errno), listenSock_ );

        //2.bind绑定
    // 2.1填充服务器信息

        struct sockaddr_in local;//用户栈

        memset(&local, 0, sizeof(local));
        local.sin_family = PF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));

         //2.2本地socket信息写入内核区域。
        if(bind(listenSock_ , (const struct sockaddr* )&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }

        logMessage(DEBUG, "bind: %s, %d", strerror(errno), listenSock_ );

        //3. 监听socket， 因为tcp是面向连接的。

        if(listen(listenSock_ , 5) < 0)
        {
            logMessage(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(DEBUG, "listen: %s, %d", strerror(errno), listenSock_ );
        //别人来连接你
    }

    //线程例程函数
    static void* threadRoutine(void* args)
    {
        pthread_detach(pthread_self());//设置线程分离
        ThreadData* td = static_cast<ThreadData*>(args);
        td->this_->transService(td->sock_, td->clinetIp_,td->clientPort_);
        delete td;
        return nullptr;
    }








    /*
    int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
```
- `listenSock_` 是之前通过 `listen()` 函数监听的套接字描述符，表示服务器的监听套接字。
- `(struct sockaddr *)&peer` 是一个指向 `peer` 结构体的指针，用于接收客户端的地址信息。
- `&len` 是一个指向 `len` 变量的指针，用于传递 `peer` 结构体的大小。

``accept()` 函数会阻塞等待客户端的连接请求，当有客户端连接时，它会返回一个新的套接字描述符 `serviceSock`，用于与客户端进行通信。通过这个新的套接字，服务器可以与客户端进行数据的收发操作。*/
    void loop()
    {
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            //4.获取链接，
            int serviceSock = accept(listenSock_, (struct sockaddr*)&peer, &len);
            if(serviceSock < 0)
            {
                //获取链接失败
                logMessage(WARINING, "accept: %s[%d]", strerror(errno), serviceSock);
                continue;//失败后重新连接
            }
            //4.1获取客户端基本信息
            uint16_t peerPort = ntohs(peer.sin_port);
            std::string peerIp = inet_ntoa(peer.sin_addr);
            //打印信息
            logMessage(DEBUG, "accept: %s | %s[%d], socket fd: %d",
                       strerror(errno), peerIp.c_str(), peerPort, serviceSock);
            //5.提供服务
            //5.2多线程版本
            ThreadData* td = new ThreadData(peerPort, peerIp, serviceSock, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, threadRoutine, (void*)td);
        }

    }





        // 大小写转化服务
    // TCP && UDP: 支持全双工
    void transService(int sock, const std::string &clientIp, uint16_t clientPort)
    {
        assert(sock >= 0);
        assert(!clientIp.empty());
        assert(clientPort >= 1024);

        char inbuffer[BUFFER_SIZE];
        while (true)
        {
            ssize_t s = read(sock, inbuffer, sizeof(inbuffer) - 1); //我们认为我们读到的都是字符串
            if (s > 0)
            {
                // read success
                inbuffer[s] = '\0';
                if(strcasecmp(inbuffer, "quit") == 0)
                {
                    logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                    break;
                }
                logMessage(DEBUG, "trans before: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);
                // 可以进行大小写转化了
                for(int i = 0; i < s; i++)
                {
                    if(isalpha(inbuffer[i]) && islower(inbuffer[i])) 
                        inbuffer[i] = toupper(inbuffer[i]);
                }
                logMessage(DEBUG, "trans after: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);

                write(sock, inbuffer, strlen(inbuffer));
            }
            else if (s == 0)
            {
                // pipe: 读端一直在读，写端不写了，并且关闭了写端，读端会如何？s == 0，代表对端关闭
                // s == 0: 代表对方关闭,client 退出
                logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                break;
            }
            else
            {
                logMessage(DEBUG, "%s[%d] - read: %s", clientIp.c_str(), clientPort, strerror(errno));
                break;
            }
        }

        // 只要走到这里，一定是client退出了，服务到此结束
        close(sock); // 如果一个进程对应的文件fd，打开了没有被归还，文件描述符泄漏！
        logMessage(DEBUG, "server close %d done", sock);
    }







private:
    int listenSock_;      //套接字
    uint16_t port_; //端口号
    std::string ip_;//IP地址
};

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
    std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n" << std::endl;

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
    {
        ip = argv[2];
    }
    ServerTcp svr(port, ip);
    svr.init();//初始化
    svr.loop();//循环

    return 0;
}