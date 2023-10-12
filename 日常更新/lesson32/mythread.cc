#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
 #include "Lock.hpp"
#include <mutex>

using namespace std;

#define NAMESIZE 64
static pthread_mutex_t mutexp = PTHREAD_MUTEX_INITIALIZER;

// typedef struct threadData
// {
//     char name[NAMESIZE];
//     pthread_mutex_t* mutexp;
// } threadData;
int tickets = 1000;

// void* startRoutine(void* args)
// {
//     const char* name = static_cast<const char*>(args);

//     while(true)
//     {
//         pthread_mutex_lock(&mutexp);
//         if(tickets > 0)
//         {
//             // usleep(1001);
//             cout << name << " 得到一张票：" << tickets << endl;
//             tickets--;
//             pthread_mutex_unlock(&mutexp);

//              usleep(500);
//         }
//         else
//         {
//             pthread_mutex_unlock(&mutexp);
//             break;
        
//         }
//     }

//     return nullptr;
// }

Mutex mymutex;
bool getTickets()
{
    bool ret = false;
    LockGuard lockguard(&mymutex);
    if(tickets > 0)
    {
        // usleep(1000);
        // cout << "thread: " << pthread_self() << "拿到一张票了" << tickets << endl;
        tickets--;
        ret = true;
    }

    return ret;
}

void* startRoutine(void* args)
{
    const char* name = static_cast<const char*>(args);
    while(true)
    {
        if(!getTickets())
        {
            break;
        }
        cout << name << "拿到一张票" << endl;

        // usleep(1000);
    }
}
int main()
{
    
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;


    // threadData* td = new threadData();
    // strcpy(td->name, "thread 0");
    // td->mutexp = &mutex;
    
    pthread_create(&t1, nullptr, startRoutine, (void*)"thread 1");
    pthread_create(&t2, nullptr, startRoutine, (void*)"thread 2");
    pthread_create(&t3, nullptr, startRoutine, (void*)"thread 3");
    pthread_create(&t4, nullptr, startRoutine, (void*)"thread 4");

    // pthread_create(&t1, nullptr, startRoutine, td);
    // pthread_create(&t2, nullptr, startRoutine, td);
    // pthread_create(&t3, nullptr, startRoutine, td);
    // pthread_create(&t4, nullptr, startRoutine, td);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);

    // delete td;
}