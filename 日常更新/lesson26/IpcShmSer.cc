#include "Comm.hpp"
#include "Log.hpp"

#include <unistd.h>

using namespace std;

//我想创建新的共享内存
const int flags = IPC_CREAT | IPC_EXCL;

//充当使用共享内存的角色
int main()
{
    
    CreateFifo();
    int fd = Open(FIFO_FILE, READER);
    assert(fd >= 0);
    printf("成功\n");
    key_t key = CreateKey();
    Log() << "key: " << key << "\n";

    Log() << "创建共享内存开始\n";
    int shmid = shmget(key, MEM_SIZE, flags | 0666);
    if(shmid < 0)
    {
        Log() << "shmget: " << strerror(errno) << "\n";
        return 2;
    }
    Log() << "create shm success, shmid: " << shmid << "\n";

        // 1. 将共享内存和自己的进程产生关联attach
    char *str = (char *)shmat(shmid, nullptr, 0);
    Log() << "attach shm : " << shmid << " success\n";


    
    // sleep(5);
    while(true)
    {
        if(Wait(fd) <= 0)  break;
        printf("%s\n", str);
        sleep(1);
    }
         // 2. 去关联
    shmdt(str);
    Log() << "detach shm : " << shmid << " success\n";
    // 删它
    shmctl(shmid, IPC_RMID, nullptr);

    Log() << "delete shm : " << shmid << " success\n";

    Close(fd, FIFO_FILE);
    // sleep(5);
    return 0;

}