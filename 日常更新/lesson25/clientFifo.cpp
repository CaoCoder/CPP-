#include "comm.h"

using namespace std;

int main()
{

    int pipeFd = open(IPC_PATH, O_WRONLY);
    if(pipeFd < 0)
    {
        cerr << "open: " << strerror(errno) << endl;
        return 1;
    }

#define NUM 1024
    char line[NUM];
    while(true)
    {
        printf("请输入你的消息# ");
        fflush(stdout);
        memset(line, 0, sizeof(line));
        if(fgets(line, sizeof(line), stdin) != NULL)
        {
            line[strlen(line)-1] = '\0';
            write(pipeFd,line,sizeof(line));
        }
        else{
            break;
        }
    }
    close(pipeFd);
    cout << "客户端退出啦" <<endl;
    return 0;
}