// #include "util.hpp"
// #include "Task.hpp"
// #include "ThreadPool.hpp"
// #include "daemonize.hpp"

// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <pthread.h>

// class ServerTcp;
// class ThreadData
// {
// public:
//     uint16_t clientPort_;//客户端端口号
//     std::string clinetIp_;//客户端ip
//     int sock_;//套接字
//     ServerTcp *this_;//服务器对象指针。
// public:
//     ThreadData(uint16_t port, std::string ip, int sock,  ServerTcp *ts)
//         : clientPort_(port), clinetIp_(ip), sock_(sock),this_(ts)
//     {}
// };
// /*
// ***服务端的主要工作是创建服务端对象，然后让对象调用初始化init函数()进行bind服务器listensock 和 监听客户端的sock
// ***init函数的工作 
// ***1.创建监听套接字listenSock_
// ***2.将 监听套接字listenSock_ 和  服务端的sockaddr_in 信息绑定bind起来。
// ***3.监听客户端的套接字
// ***然后让对象调用loop()函数
// ***1.调用accept函数进行获取连接.(通过从 监听套接字listenSock 和 获取的客户端sockaddr_in peer信息 来返回一个服务端套接字serviceSock来为这一个 客户端 提供服务)。
// ***2.通过accept连接客户端得到的的serviceSock这个套接字，对客户端进行提供服务 transService英汉互译服务
// ***3.transService先从这个accept连接函数得到的套接字进行读取，然后进行大小写转换，再把这个结果write回serviceSock中。
// */
// class ServerTcp
// {
// public:
//     ServerTcp(uint16_t port,const std::string& ip = " " )
//         :port_(port)
//         ,ip_(ip)
//         ,listenSock_(-1)
//         ,tp_(nullptr)
//         {}
//     ~ServerTcp()
//     {

//     }
// public:
//       // 大小写转化服务
//     //使用tcp或udp
//     //套接字 客户端ip 客户端port
//     void transService(int sock, const std::string& clientIp, uint16_t clientPort)
//     {
//         assert(sock >= 0);
//         assert(!clientIp.empty());
//         assert(clientPort >= 1024);//端口号最好大于1000

//         char inbuffer[BUFFER_SIZE];
//         while(true)
//         {
//             //从套接字中读取消息
//             ssize_t s = read(sock, inbuffer, sizeof(inbuffer)- 1);
//             if(s > 0)
//             {
//                 //read success
//                 inbuffer[s] = '\0';
//                 //不考虑大小写
//                 if(strcasecmp(inbuffer, "quit") == 0)
//                 {
//                     logMessage(DEBUG, "client quit -- %s[%d]",clientIp.c_str(), clientPort);
//                     break;
//                 }
//                 logMessage(DEBUG, "trans before: %s[%d]>>>%s", clientIp.c_str(),clientPort, inbuffer);
//                 //读取上来可以转换大小写了。
//                 for(int i = 0; i < s; i++)
//                 {
//                     //如果是字母并且是小写
//                     if(isalpha(inbuffer[i]) && islower(inbuffer[i]))
//                     {
//                         //转化为大写，并重新写入源字符串
//                         inbuffer[i] = toupper(inbuffer[i]);
//                     }
//                 }
//                 logMessage(DEBUG, "trans after: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);

//                 //写入到套接字中？？
//                 //重新写入sock中
//                 write(sock, inbuffer, strlen(inbuffer));
                
//             }
//             else if (s == 0)
//             {
//                 //s == 0表示对端关闭
//                 //pipe: 读端一直在读，写端不写了，关闭了写端，读端也关闭掉。
//                  logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
//                  break;//退出服务
//             }
//             else
//             {
//                 logMessage(DEBUG, "%s[%d] - read: %s", clientIp.c_str(), clientPort, strerror(errno));
//                 break;//退出服务

//             }
//         }

//         //只要走到这里，一定是client退出了，服务到此结束。
//         close(sock);//如果一个进程对应的文件fd，打开了没有被归还，文件描述符会泄露
//         //文件描述符是有限的。
//          logMessage(DEBUG, "server close %d done", sock);
//     }
//     void init()
//     {
//         //1.创建socket （监听套接字）
//         listenSock_ = socket(AF_INET, SOCK_STREAM, 0);
//         if(listenSock_  < 0)
//         {
//             logMessage(FATAL, "socket: %s", strerror(errno));
//             exit(SOCKET_ERR);
//         }

//         logMessage(DEBUG, "socket: %s, %d", strerror(errno), listenSock_ );

//         //2.bind绑定
//     // 2.1填充服务器信息

//         struct sockaddr_in local;//用户栈

//         memset(&local, 0, sizeof(local));
//         local.sin_family = PF_INET;
//         local.sin_port = htons(port_);
//         ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));

//          //2.2本地socket信息写入内核区域。
//         if(bind(listenSock_ , (const struct sockaddr* )&local, sizeof(local)) < 0)
//         {
//             logMessage(FATAL, "bind: %s", strerror(errno));
//             exit(BIND_ERR);
//         }

//         logMessage(DEBUG, "bind: %s, %d", strerror(errno), listenSock_ );

//         //3. 监听socket， 因为tcp是面向连接的。

//         if(listen(listenSock_ , 5) < 0)
//         {
//             logMessage(FATAL, "listen: %s", strerror(errno));
//             exit(LISTEN_ERR);
//         }
//         logMessage(DEBUG, "listen: %s, %d", strerror(errno), listenSock_ );
//         //别人来连接你



//           //4.加载线程池
//         tp_ = ThreadPool<Task>::getInstance();
//     }

//     //线程例程函数
//     // static void* threadRoutine(void* args)
//     // {
//     //     pthread_detach(pthread_self());//设置线程分离
//     //     ThreadData* td = static_cast<ThreadData*>(args);
//     //    //为了能够在静态函数中访问类成员函数，需要通过指针访问。
//     //     //设置成static可以让所有对象共享
//     //     td->this_->transService(td->sock_, td->clinetIp_,td->clientPort_);
//     //     delete td;
//     //     return nullptr;
//     // }

  






//     /*
//     int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
// ```
// - `listenSock_` 是之前通过 `listen()` 函数监听的套接字描述符，表示服务器的监听套接字。
// - `(struct sockaddr *)&peer` 是一个指向 `peer` 结构体的指针，用于接收客户端的地址信息。
// - `&len` 是一个指向 `len` 变量的指针，用于传递 `peer` 结构体的大小。

// ``accept()` 函数会阻塞等待客户端的连接请求，当有客户端连接时，它会返回一个新的套接字描述符 `serviceSock`，用于与客户端进行通信。通过这个新的套接字，服务器可以与客户端进行数据的收发操作。*/
//     void loop()
//     {
//         /*通过调用signal(SIGCHLD, SIG_IGN)，父进程将SIGCHLD信号的处理方式设置为SIG_IGN，即忽略该信号。这样，在子进程退出时，操作系统会自动回收子进程的资源，
//         避免产生僵尸进程。父进程无需显式地调用waitpid()来回收子进程，从而简化了代码。*/
//         // signal(SIGCHLD, SIG_IGN);

//         tp_->start();//启动线程池，创建线程
//          logMessage(DEBUG, "thread pool start success, thread num: %d", tp_->threadNum());
//         while(true)
//         {
//             struct sockaddr_in peer;
//             socklen_t len = sizeof(peer);
//             //4.获取链接，
//             int serviceSock = accept(listenSock_, (struct sockaddr*)&peer, &len);
//             if(serviceSock < 0)
//             {
//                 //获取链接失败
//                 logMessage(WARINING, "accept: %s[%d]", strerror(errno), serviceSock);
//                 continue;//失败后重新连接
//             }
//             //4.1获取客户端基本信息
//             uint16_t peerPort = ntohs(peer.sin_port);
//             std::string peerIp = inet_ntoa(peer.sin_addr);
//             //打印信息
//             logMessage(DEBUG, "accept: %s | %s[%d], socket fd: %d",
//                        strerror(errno), peerIp.c_str(), peerPort, serviceSock);
//             //5.提供服务

//                 //5.1多进程--版本
// //             pid_t id = fork();
// //             assert(id != -1);
// //             if(id == 0)
// //             {
// //                 close(listenSock_);//建议关闭
// //                 //子进程
// //                 transService(serviceSock, peerIp, peerPort);
// //                 exit(0);//进入僵尸进程
// //             }  
// //             /*在调用fork()函数后，子进程会复制父进程的所有资源，包括套接字。
// // 子进程中也有一个对应的serviceSock副本，但在子进程中并不需要使用这个套接字来接受新的连接。
// // 如果父进程不关闭serviceSock，那么在父进程和子进程中都存在这个套接字的引用，可能会导致资源泄露或意外的行为。
// // 通过在父进程中执行close(serviceSock)，父进程关闭了这个套接字，从而确保只有子进程在使用serviceSock。*/
// //                  // 父进程
// //              close(serviceSock); //这一步是一定要做的！
//             //  pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
//             // assert(ret > 0);
//             // (void)ret;

//             //5.2多线程版本
//             // ThreadData* td = new ThreadData(peerPort, peerIp, serviceSock, this);
//             // pthread_t tid;
//             // pthread_create(&tid, nullptr, threadRoutine, (void*)td);

//             //v5.3版本，爷孙多进程
//                         //爷爷进程
//                         /*
//                         为什么要使用这种方式来创建孤儿进程呢？这是为了简化父进程对子进程的回收操作。通过创建孤儿进程，爷爷进程（原始的父进程）可以继续运行并监听新的连接，而不需要关心孙子进程的回收。
//                         爷爷进程在等待子进程退出时使用了waitpid()函数，确保爸爸进程被正确回收。*/
//             // pid_t id = fork();
//             // if(id == 0)
//             // {
//             //     // 爸爸进程
//             //     close(listenSock_);//建议
//             //     // 又进行了一次fork，让 爸爸进程
//             //     if(fork() > 0) exit(0);//创建爸爸进程就退出，然后孙子进程就变为了孤儿进程。
//             //     // 孙子进程 -- 就没有爸爸 -- 孤儿进程 -- 被系统领养 -- 回收问题就交给了系统来回收
//             //     transService(serviceSock, peerIp, peerPort);
//             //     exit(0);
//             // }
//             // // 父进程
//             // close(serviceSock); //这一步是一定要做的！
           
//             // pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
//             // assert(ret > 0);
//             // (void)ret;

//                         // 5.3 v3 版本 --- 线程池版本
//             // 5.3.1 构建任务
//             // 5.3 v3.1
//              Task t(serviceSock, peerIp, peerPort, std::bind(&ServerTcp::transService, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//              tp_->push(t);

//             // 5.3 v3.2
//             //Task t(serviceSock, peerIp, peerPort, transService);
//             //tp_->push(t);
//             // 5.3 v3.3
//             // Task t(serviceSock, peerIp, peerPort, execCommand);
//             // tp_->push(t);


//         }

//     }





      







// private:
//     int listenSock_;      //套接字
//     uint16_t port_; //端口号
//     std::string ip_;//IP地址

//     //引入线程池
//     ThreadPool<Task>* tp_;
// };

// static void Usage(std::string proc)
// {
//     std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
//     std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n" << std::endl;

// }


// int main(int argc, char* argv[])
// {
//     if(argc != 2 && argc != 3)
//     {
//         Usage(argv[0]);
//         exit(USAGE_ERR);
//     }
//     uint16_t port = atoi(argv[1]);
//     std::string ip;
//     if(argc == 3)
//     {
//         ip = argv[2];
//     }
//     ServerTcp svr(port, ip);//创建服务端对象
//     svr.init();//初始化
//     svr.loop();//循环

//     return 0;
// }
#include "util.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"
#include "daemonize.hpp"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

class ServerTcp; // 申明一下ServerTcp

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
            if (strcasecmp(inbuffer, "quit") == 0)
            {
                logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                break;
            }
            logMessage(DEBUG, "trans before: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);
            // 可以进行大小写转化了
            for (int i = 0; i < s; i++)
            {
                if (isalpha(inbuffer[i]) && islower(inbuffer[i]))
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

void execCommand(int sock, const std::string &clientIp, uint16_t clientPort)
{
    assert(sock >= 0);
    assert(!clientIp.empty());
    assert(clientPort >= 1024);

    char command[BUFFER_SIZE];
    while (true)
    {
        ssize_t s = read(sock, command, sizeof(command) - 1); //我们认为我们读到的都是字符串
        if (s > 0)
        {
            command[s] = '\0';
            logMessage(DEBUG, "[%s:%d] exec [%s]", clientIp.c_str(), clientPort, command);
            // 考虑安全
            std::string safe = command;
            if((std::string::npos != safe.find("rm")) || (std::string::npos != safe.find("unlink")))
            {
                break;
            }
            // 我们是以r方式打开的文件，没有写入
            // 所以我们无法通过dup的方式得到对应的结果
            FILE *fp = popen(command, "r");
            if(fp == nullptr)
            {
                logMessage(WARINING, "exec %s failed, beacuse: %s", command, strerror(errno));
                break;
            }
            char line[1024];
            while(fgets(line, sizeof(line)-1, fp) != nullptr)
            {
                write(sock, line, strlen(line));
            }
            // dup2(fd, 1);
            // dup2(sock, fp->_fileno);
            // fflush(fp);
            pclose(fp);
            logMessage(DEBUG, "[%s:%d] exec [%s] ... done", clientIp.c_str(), clientPort, command);
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

class ThreadData
{
public:
    uint16_t clientPort_;
    std::string clinetIp_;
    int sock_;
    ServerTcp *this_;

public:
    ThreadData(uint16_t port, std::string ip, int sock, ServerTcp *ts)
        : clientPort_(port), clinetIp_(ip), sock_(sock), this_(ts)
    {
    }
};

class ServerTcp
{
public:
    ServerTcp(uint16_t port, const std::string &ip = "")
        : port_(port),
          ip_(ip),
          listenSock_(-1),
          tp_(nullptr)
    {
    }
    ~ServerTcp()
    {
    }

public:
    void init()
    {
        // 1. 创建socket
        listenSock_ = socket(PF_INET, SOCK_STREAM, 0);
        if (listenSock_ < 0)
        {
            logMessage(FATAL, "socket: %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        logMessage(DEBUG, "socket: %s, %d", strerror(errno), listenSock_);

        // 2. bind绑定
        // 2.1 填充服务器信息
        struct sockaddr_in local; // 用户栈
        memset(&local, 0, sizeof local);
        local.sin_family = PF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));
        // 2.2 本地socket信息，写入sock_对应的内核区域
        if (bind(listenSock_, (const struct sockaddr *)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }
        logMessage(DEBUG, "bind: %s, %d", strerror(errno), listenSock_);

        // 3. 监听socket，为何要监听呢？tcp是面向连接的！
        if (listen(listenSock_, 5 /*后面再说*/) < 0)
        {
            logMessage(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(DEBUG, "listen: %s, %d", strerror(errno), listenSock_);
        // 运行别人来连接你了

        // 4. 加载线程池
        tp_ = ThreadPool<Task>::getInstance();
    }
    // static void *threadRoutine(void *args)
    // {
    //     pthread_detach(pthread_self()); //设置线程分离
    //     ThreadData *td = static_cast<ThreadData *>(args);
    //     td->this_->transService(td->sock_, td->clinetIp_, td->clientPort_);
    //     delete td;
    //     return nullptr;
    // }
    void loop()
    {
        // signal(SIGCHLD, SIG_IGN); // only Linux
        tp_->start();
        logMessage(DEBUG, "thread pool start success, thread num: %d", tp_->threadNum());
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 4. 获取连接, accept 的返回值是一个新的socket fd ？？
            // 4.1 listenSock_: 监听 && 获取新的链接-> sock
            // 4.2 serviceSock: 给用户提供新的socket服务
            int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
            if (serviceSock < 0)
            {
                // 获取链接失败
                logMessage(WARINING, "accept: %s[%d]", strerror(errno), serviceSock);
                continue;
            }
            // 4.1 获取客户端基本信息
            uint16_t peerPort = ntohs(peer.sin_port);
            std::string peerIp = inet_ntoa(peer.sin_addr);

            logMessage(DEBUG, "accept: %s | %s[%d], socket fd: %d",
                       strerror(errno), peerIp.c_str(), peerPort, serviceSock);
            // 5 提供服务, echo -> 小写 -> 大写
            // 5.0 v0 版本 -- 单进程 -- 一旦进入transService，主执行流，就无法进行向后执行，只能提供完毕服务之后才能进行accept
            // transService(serviceSock, peerIp, peerPort);

            // 5.1 v1 版本 -- 多进程版本 -- 父进程打开的文件会被子进程继承吗？会的
            // pid_t id = fork();
            // assert(id != -1);
            // if(id == 0)
            // {
            //     close(listenSock_); //建议
            //     //子进程
            //     transService(serviceSock, peerIp, peerPort);
            //     exit(0); // 进入僵尸
            // }
            // // 父进程
            // close(serviceSock); //这一步是一定要做的！

            // 5.1 v1.1 版本 -- 多进程版本  -- 也是可以的
            // 爷爷进程
            // pid_t id = fork();
            // if(id == 0)
            // {
            //     // 爸爸进程
            //     close(listenSock_);//建议
            //     // 又进行了一次fork，让 爸爸进程
            //     if(fork() > 0) exit(0);
            //     // 孙子进程 -- 就没有爸爸 -- 孤儿进程 -- 被系统领养 -- 回收问题就交给了系统来回收
            //     transService(serviceSock, peerIp, peerPort);
            //     exit(0);
            // }
            // // 父进程
            // close(serviceSock); //这一步是一定要做的！
            // // 爸爸进程直接终止，立马得到退出码，释放僵尸进程状态
            // pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
            // assert(ret > 0);
            // (void)ret;

            // 5.2 v2 版本 -- 多线程
            // 这里不需要进行关闭文件描述符吗？？不需要啦
            // 多线程是会共享文件描述符表的！
            // ThreadData *td = new ThreadData(peerPort, peerIp, serviceSock, this);
            // pthread_t tid;
            // pthread_create(&tid, nullptr, threadRoutine, (void*)td);

            // 5.3 v3 版本 --- 线程池版本
            // 5.3.1 构建任务
            // 5.3 v3.1
            // Task t(serviceSock, peerIp, peerPort, std::bind(&ServerTcp::transService, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            // tp_->push(t);

            // 5.3 v3.2
            // Task t(serviceSock, peerIp, peerPort, transService);
            // tp_->push(t);
            // 5.3 v3.3
            Task t(serviceSock, peerIp, peerPort, execCommand);
            tp_->push(t);

            // waitpid(); 默认是阻塞等待！WNOHANG
            // 方案1

            // logMessage(DEBUG, "server 提供 service start ...");
            // sleep(1);
        }
    }

private:
    // sock
    int listenSock_;
    // port
    uint16_t port_;
    // ip
    std::string ip_;
    // 引入线程池
    ThreadPool<Task> *tp_;
};

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
    std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n"
              << std::endl;
}

// ./ServerTcp local_port local_ip
int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::string ip;
    if (argc == 3)
        ip = argv[2];
    
    daemonize(); // 我们的进程就会成为守护进程

    ServerTcp svr(port, ip);
    svr.init();
    svr.loop();
    return 0;
}
