#include "util.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"
#include "daemonize.hpp"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

class ServerTcp;
class ThreadData
{
public:
    uint16_t clientPort_;//客户端端口号
    std::string clinetIp_;//客户端ip
    int sock_;//套接字
    ServerTcp *this_;//服务器对象指针。
public:
    ThreadData(uint16_t port, std::string ip, int sock,  ServerTcp *ts)
        : clientPort_(port), clinetIp_(ip), sock_(sock),this_(ts)
    {}
};
/*
***服务端的主要工作是创建服务端对象，然后让对象调用初始化init函数()进行bind服务器listensock 和 监听客户端的sock
***init函数的工作 
***1.创建监听套接字listenSock_
***2.将 监听套接字listenSock_ 和  服务端的sockaddr_in 信息绑定bind起来。
***3.监听客户端的套接字
***然后让对象调用loop()函数
***1.调用accept函数进行获取连接.(通过从 监听套接字listenSock 和 获取的客户端sockaddr_in peer信息 来返回一个服务端套接字serviceSock来为这一个 客户端 提供服务)。
***2.通过accept连接客户端得到的的serviceSock这个套接字，对客户端进行提供服务 transService英汉互译服务
***3.transService先从这个accept连接函数得到的套接字进行读取，然后进行大小写转换，再把这个结果write回serviceSock中。
*/

// 大小写转化服务
    //使用tcp或udp
    //套接字 客户端ip 客户端port
    void transService(int sock, const std::string& clientIp, uint16_t clientPort)
    {
        assert(sock >= 0);
        assert(!clientIp.empty());
        assert(clientPort >= 1024);//端口号最好大于1000

        char inbuffer[BUFFER_SIZE];
        while(true)
        {
            //从套接字中读取消息
            ssize_t s = read(sock, inbuffer, sizeof(inbuffer)- 1);
            if(s > 0)
            {
                //read success
                inbuffer[s] = '\0';
                //不考虑大小写
                if(strcasecmp(inbuffer, "quit") == 0)
                {
                    logMessage(DEBUG, "client quit -- %s[%d]",clientIp.c_str(), clientPort);
                    break;
                }
                logMessage(DEBUG, "trans before: %s[%d]>>>%s", clientIp.c_str(),clientPort, inbuffer);
                //读取上来可以转换大小写了。
                for(int i = 0; i < s; i++)
                {
                    //如果是字母并且是小写
                    if(isalpha(inbuffer[i]) && islower(inbuffer[i]))
                    {
                        //转化为大写，并重新写入源字符串
                        inbuffer[i] = toupper(inbuffer[i]);
                    }
                }
                logMessage(DEBUG, "trans after: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);

                //写入到套接字中？？
                //重新写入sock中
                write(sock, inbuffer, strlen(inbuffer));
                
            }
            else if (s == 0)
            {
                //s == 0表示对端关闭
                //pipe: 读端一直在读，写端不写了，关闭了写端，读端也关闭掉。
                 logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                 break;//退出服务
            }
            else
            {
                logMessage(DEBUG, "%s[%d] - read: %s", clientIp.c_str(), clientPort, strerror(errno));
                break;//退出服务

            }
        }

        //只要走到这里，一定是client退出了，服务到此结束。
        close(sock);//如果一个进程对应的文件fd，打开了没有被归还，文件描述符会泄露
        //文件描述符是有限的。
         logMessage(DEBUG, "server close %d done", sock);
    }


void execCommand(int sock, const std::string& clientIp, uint16_t clientPort)
{
    assert(sock >= 0);
    assert(!clientIp.empty());
    assert(clientPort >= 1024);

    char command[BUFFER_SIZE];
    while(true)
    {
        //从提供服务的套接字读取数据
        ssize_t s = read(sock, command, sizeof(command) - 1);
        if(s > 0)
        {
            command[s] = '\0';
             logMessage(DEBUG, "[%s:%d] exec [%s]", clientIp.c_str(), clientPort, command);
            //调用 popopen() 函数执行 command 命令，
            //并通过管道获取命令的输出结果。返回的文件指针 fp 用于读取命令的输出。
            FILE* fp = popen(command, "r");
            if(fp == nullptr)
            {
                logMessage(WARINING, "exec %s failed, beacuse: %s", command, strerror(errno));
                break;
            }
            /*
            使用 fgets() 函数从 fp 中逐行读取命令的输出结果，
            并将每行内容发送回客户端，通过 write() 函数将数据写入 sock。
            循环读取命令输出的每一行，直到 fgets() 返回 nullptr
            */
            char line[1024];
            while(fgets(line, sizeof(line)-1, fp) != nullptr)
            {
                write(sock, line, strlen(line));//写回客户端
            }

            pclose(fp);
            logMessage(DEBUG, "[%s:%d] exec [%s] ... done", clientIp.c_str(), clientPort, command);
        }
        else if(s == 0)//代表写端关闭了，
        {
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


class ServerTcp
{
public:
    ServerTcp(uint16_t port,const std::string& ip = " " )
        :port_(port)
        ,ip_(ip)
        ,listenSock_(-1)
        ,tp_(nullptr)
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



          //4.加载线程池
        tp_ = ThreadPool<Task>::getInstance();
    }

    //线程例程函数
    // static void* threadRoutine(void* args)
    // {
    //     pthread_detach(pthread_self());//设置线程分离
    //     ThreadData* td = static_cast<ThreadData*>(args);
    //    //为了能够在静态函数中访问类成员函数，需要通过指针访问。
    //     //设置成static可以让所有对象共享
    //     td->this_->transService(td->sock_, td->clinetIp_,td->clientPort_);
    //     delete td;
    //     return nullptr;
    // }

  






    /*
    int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
```
- `listenSock_` 是之前通过 `listen()` 函数监听的套接字描述符，表示服务器的监听套接字。
- `(struct sockaddr *)&peer` 是一个指向 `peer` 结构体的指针，用于接收客户端的地址信息。
- `&len` 是一个指向 `len` 变量的指针，用于传递 `peer` 结构体的大小。

``accept()` 函数会阻塞等待客户端的连接请求，当有客户端连接时，它会返回一个新的套接字描述符 `serviceSock`，用于与客户端进行通信。通过这个新的套接字，服务器可以与客户端进行数据的收发操作。*/
    void loop()
    {
        /*通过调用signal(SIGCHLD, SIG_IGN)，父进程将SIGCHLD信号的处理方式设置为SIG_IGN，即忽略该信号。这样，在子进程退出时，操作系统会自动回收子进程的资源，
        避免产生僵尸进程。父进程无需显式地调用waitpid()来回收子进程，从而简化了代码。*/
        // signal(SIGCHLD, SIG_IGN);

        tp_->start();//启动线程池，创建线程
         logMessage(DEBUG, "thread pool start success, thread num: %d", tp_->threadNum());

         //线程有了，菜齐了，开始让listenSock揽客
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

                //5.1多进程--版本
//             pid_t id = fork();
//             assert(id != -1);
//             if(id == 0)
//             {
//                 close(listenSock_);//建议关闭
//                 //子进程
//                 transService(serviceSock, peerIp, peerPort);
//                 exit(0);//进入僵尸进程
//             }  
//             /*在调用fork()函数后，子进程会复制父进程的所有资源，包括套接字。
// 子进程中也有一个对应的serviceSock副本，但在子进程中并不需要使用这个套接字来接受新的连接。
// 如果父进程不关闭serviceSock，那么在父进程和子进程中都存在这个套接字的引用，可能会导致资源泄露或意外的行为。
// 通过在父进程中执行close(serviceSock)，父进程关闭了这个套接字，从而确保只有子进程在使用serviceSock。*/
//                  // 父进程
//              close(serviceSock); //这一步是一定要做的！
            //  pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
            // assert(ret > 0);
            // (void)ret;

            //5.2多线程版本
            // ThreadData* td = new ThreadData(peerPort, peerIp, serviceSock, this);
            // pthread_t tid;
            // pthread_create(&tid, nullptr, threadRoutine, (void*)td);

            //v5.3版本，爷孙多进程
                        //爷爷进程
                        /*
                        为什么要使用这种方式来创建孤儿进程呢？这是为了简化父进程对子进程的回收操作。通过创建孤儿进程，爷爷进程（原始的父进程）可以继续运行并监听新的连接，而不需要关心孙子进程的回收。
                        爷爷进程在等待子进程退出时使用了waitpid()函数，确保爸爸进程被正确回收。*/
            // pid_t id = fork();
            // if(id == 0)
            // {
            //     // 爸爸进程
            //     close(listenSock_);//建议
            //     // 又进行了一次fork，让 爸爸进程
            //     if(fork() > 0) exit(0);//创建爸爸进程就退出，然后孙子进程就变为了孤儿进程。
            //     // 孙子进程 -- 就没有爸爸 -- 孤儿进程 -- 被系统领养 -- 回收问题就交给了系统来回收
            //     transService(serviceSock, peerIp, peerPort);
            //     exit(0);
            // }
            // // 父进程
            // close(serviceSock); //这一步是一定要做的！
           
            // pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
            // assert(ret > 0);
            // (void)ret;

                        // 5.3 v3 版本 --- 线程池版本
            // 5.3.1 构建任务
            // 5.3 v3.1
             //Task t(serviceSock, peerIp, peerPort, std::bind(&ServerTcp::transService, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
             //tp_->push(t);

            // 5.3 v3.2
            //创建任务，(想吃的菜)
            //Task t(serviceSock, peerIp, peerPort, transService);
            //tp_->push(t);//把菜推给线程池，。
            // 5.3 v3.3
             Task t(serviceSock, peerIp, peerPort, execCommand);
             tp_->push(t);


        }

    }





      







private:
    int listenSock_;      //套接字
    uint16_t port_; //端口号
    std::string ip_;//IP地址

    //引入线程池
    ThreadPool<Task>* tp_;
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

    /*
    如果在调用 daemonize() 函数之后杀死父进程，那么子进程（即 TCP 服务器）会成为孤儿进程，并由 init 进程接管。
    然而，由于子进程是通过 fork() 创建的，它继承了父进程的文件描述符和运行状态，所以在杀死父进程后，子进程仍然会继续运行。

    因此，杀死父进程不会立即结束这个 TCP 服务器进程。
    相反，子进程会继续执行后续的代码，创建 TCP 服务器对象，
    进行初始化，并进入循环运行状态。只有当子进程自行退出或者因为其他原因被终止时，TCP 服务器进程才会结束。
    */
    //daemonize();//成为守护进程

    ServerTcp svr(port, ip);//创建服务端对象
    svr.init();//初始化
    svr.loop();//循环
    
    return 0;
}
