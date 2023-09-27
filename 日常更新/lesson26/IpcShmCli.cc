#include "Comm.hpp"
#include "Log.hpp"

#include <cstdio>
#include <unistd.h>

using namespace std;

//充当使用共享内存的角色
int main()
{
    int fd = Open(FIFO_FILE, WRITER);

    //创建相同的key值
    key_t key = CreateKey();
    Log() << "key: " << key << "\n";
    
    //获取共享内存
    int shmid = shmget(key, MEM_SIZE, IPC_CREAT);
    if(shmid < 0)
    {
        Log() << "shmget: " << strerror(errno) << "\n";
        return 2;
    }

    //挂接
    char* str = (char*)shmat(shmid, nullptr, 0);
    //使用他
    while(true)
    {
        printf("请输入");
        fflush(stdout);
        ssize_t s = read(0, str, MEM_SIZE);
        if(s > 0)
        {
            str[s] = '\0';
        }
        Signal(fd);
    }
   
    //去关联
    shmdt(str);

    return 0;
}