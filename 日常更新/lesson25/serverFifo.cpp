#include "comm.h"

using namespace std;

int main()
{
    umask(0);
    if(mkfifo(IPC_PATH,0600) != 0)
    {
        cerr << "mkfifo crror" << endl;
        return 1;
    }

    int pipeFd = open(IPC_PATH, O_RDONLY);
    if(pipeFd < 0)
    {
        cerr << "open fifo error" << endl;
        return 2;
    }
#define NUM 1024
    //正常通信
    char buffer[NUM];
    while(true)
    {
        ssize_t s = read(pipeFd, buffer,sizeof(buffer));
        if(s > 0)
        {
            buffer[s] = '\0';
            cout << "客服端->服务器 " << buffer << endl;
        }
        else if(s==0)
        {
            cout << "客户端退出啦，我也要退出";
            break;
        }
        else{
            cout << "read: "<< strerror(errno) << endl;
            break;
        }
    }

    close(pipeFd);
    cout << "服务端退出啦" << endl;
    unlink(IPC_PATH);
    return 0;
}