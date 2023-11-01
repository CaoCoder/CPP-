#include "util.hpp"

volatile bool quit = false;


//使用手册
static void Usage(std::string proc)
{
    //进行换行和tab建
    std::cerr << "Usage: \n\t" << proc << " serverIp serverPort" << std::endl;
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

    //1.创建流式套接字 sock_stream
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        std::cerr << "socket: "<< strerror(errno) << std::endl;
        exit(SOCKET_ERR);
    }

    //2.connect, 发起连接请求， 你想谁发起请求呢？
    //2.1先填充需要连接的远端主机的基本信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverPort);//端口号主机转网络
    inet_aton(serverIp.c_str(), &server.sin_addr);//ip地址 主机转网络

    //2.2发起请求，connect会自动帮我们绑定
    if(connect(sock, (const struct sockaddr*)&server, sizeof(server)) != 0)
    {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        exit(CONN_ERR);
    }

    std::cout << "info : connect success: " << sock << std::endl;
    /*
        这段代码实现了一个简单的客户端程序，
        用户可以通过输入消息与服务器进行交互。
        用户输入的消息被写入到 sock 文件中，然后从 sock 
        文件中读取服务器的回显并打印出来。当用户输入 "quit" 时，
        程序退出循环，结束执行。
    */
    std::string message;
    while(!quit)
    {
        message.clear();
        std::cout << "请输入你的消息>>>";
        std::getline(std::cin, message);
        if(strcasecmp(message.c_str(), "quit") == 0)
        {
            quit = true;
        }
        //写入到sock文件中
        ssize_t s = write(sock, message.c_str(), message.size());
        if(s >  0 )//s是返回的字节的个数
        {
            //扩容并初始化为0
            message.resize(1024);
            //从sock读取文件大小是1024
            ssize_t s = read(sock, (char*)(message.c_str()), 1024);
            if(s > 0)
            {
                //变成C语言风格
                message[s] = 0;
                std::cout << "Server Echo>>> " << message << std::endl;
            }
            //没有读到数据
            else if(s <= 0)
            {
                break;
            }

        }
    }

    //关闭套接字
    close(sock);
    return 0;


}