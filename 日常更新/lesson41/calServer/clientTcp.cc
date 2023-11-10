
#include "util.hpp"
#include "Protocol.hpp"
#include <cstdio>
volatile bool quit = false;

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " serverIp serverPort" << std::endl;
    std::cerr << "Example:\n\t" << proc << " 127.0.0.1 8081\n"
              << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    std::string serverIp = argv[1];
    uint16_t serverPort = atoi(argv[2]);

    //1.创建流式scoket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        exit(SOCKET_ERR);
    }

    //2.connect发起连接请求

    //2.1填充服务器主机的基本信息

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;//填充协议家族
    server.sin_port = htons(serverPort);//填充端口号
    inet_aton(serverIp.c_str(), &server.sin_addr);//填充Ip地址

    //2.2发起connect请求

    if(connect(sock, (const struct sockaddr* )&server, sizeof(server) )!= 0)
    {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        exit(CONN_ERR);
    }

    std::cout << "info: connect success: " << sock << std::endl;

    std::string message;
    while(!quit)
    {
        message.clear();
        std::cout << "请输入表达式>>> " ; //1 + 1
        std::getline(std::cin, message);//结尾不会有\n
        if(strcasecmp(message.c_str(), "quit") == 0)
        {
            //不区分大小写的匹配
            quit = true;
            continue;
        }

        Request req;//定义请求对象
        if(!makeRequest(message, &req)) continue;

        std::string package;
        req.serialize(&package);//序列化为字符串 1 + 1 

        std::cout << req.x_ << req.op_ << req.y_ << std::endl;

        std::cout << "debug->serialize-> " << package << std::endl;
        package = encode(package, package.size());//加密 为字符串添加 长度 报头
        std::cout << "debug->encode-> \n" << package << std::endl;
       
        ssize_t s = write(sock, package.c_str(), package.size());//像服务端sock写入
        if(s > 0)//写入成功
        {
            char buff[1024];
            //读取服务端回复的
            /*
            write是将数据拷贝到了文件描述符对应的发送缓冲区 
             read是从文件描述符对应的接收缓冲区中读取数据  
             数据是在不同的缓冲区中
             所以此时的客户端的read是无法读到客户端发送给服务端数据
            */
           std::cout << "写入成功" << std::endl;
            size_t s = read(sock, buff, sizeof(buff) - 1);
            if(s > 0)
            {
                std::cout << "读取成功" << std::endl;
                buff[s] = 0;

            }
            std::string echoPackage = buff;//结果
            Response resp;
            uint32_t  len = 0;
            std::string tmp = decode(echoPackage, &len);//结果进行解码
            std::cout << "解码成功" << std::endl;
            if(len > 0)
            {
                echoPackage = tmp;
                resp.deserialize(echoPackage);//反序列化 为 结构体
                //退出码 结果
                printf("[exitcode: %d] %d\n", resp.exitCode_, resp.result_);
            }

            
        }
        else if(s <= 0)//对端关闭 或写入失败
        {
            break;
        }

    }
    //停止写入记得关闭sock，本质就是文件描述符。也就是文件。
    close(sock);
    return 0;
    
}
