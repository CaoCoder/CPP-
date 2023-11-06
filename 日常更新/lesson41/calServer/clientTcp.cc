
#include "util.hpp"
#include "Protocol.hpp"

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

    if(connect(sock, (const struct sockaddr* )&server, sizeof(server) != 0))
    {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        exit(CONN_ERR);
    }

    std::cout << "info: connect success: " << sock << std::endl;

    std::string message;
    while(!quit)
    {
        message.clear();
        std::cout << "请输入你的消息>>> " ;
        std::getline(std::cin, message);//结尾不会有\n
        if(strcasecmp(message.c_str(), "quit") == 0)//不区分大小写的匹配
            quit = true;

        ssize_t s = write(sock, message.c_str(), message.size());//像服务端sock写入
        if(s > 0)//写入成功
        {
            message.resize(1024);
            ssize_t s = read(sock, (char*)(message.c_str()), 1024);
            if(s > 0)
                message[s] = 0;//设置为C风格
            std::cout << "Server Echo>>> " << message << std::endl;
            
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
