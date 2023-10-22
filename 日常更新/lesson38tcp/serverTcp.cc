#include "util.hpp"


class ServerTcp
{
public:
    ServerTcp(uint16_t port,const std::string& ip = " " )
        :port_(port)
        ,ip_(ip)
        {}

public:
    void init()
    {
        //1.创建socket
        sock_ = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_ < 0)
        {
            logMessage(FATAL, "socket: %s", strerror(errno));
            exit(SOCKET_ERR);
        }

        logMessage(DEBUG, "socket: %s, %d", strerror(errno), sock_);

        //2.bind绑定
    // 2.1填充服务器信息

        struct sockaddr_in local;//用户栈

        memset(&local, 0, sizeof(local));
        local.sin_family = PF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));

         //2.2本地socket信息写入内核区域。
        if(bind(sock_, (const struct sockaddr* )&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }

        logMessage(DEBUG, "bind: %s, %d", strerror(errno), sock_);

        //3. 监听socket， 因为tcp是面向连接的。

        if(listen(sock_, 5) < 0)
        {
            logMessage(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(DEBUG, "listen: %s, %d", strerror(errno), sock_);
        //别人来连接你
    }
private:
    int sock_;      //套接字
    uint16_t port_; //端口号
    std::string ip_;//IP地址
};