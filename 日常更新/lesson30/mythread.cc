#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
using namespace std;

int global_value = 100;


static void printTid(const char* name, const pthread_t& tid)
{
    printf("%s 正在运行, thread id: 0x%x, global_value: %d\n", name, tid, global_value);
}

void* startRoutine(void* args)
{
    // const char* name = static_cast<const char*>(args);
    // int cnt = 5;
    // while (true)
    // {
    //     printTid(name, pthread_self());
    //     sleep(1);
    //     if(!(cnt--))
    //     {
    //         global_value = 200;
    //         // int* p = nullptr;
    //         // *p = 100;
    //         break;
    //     }
    // }

    cout << "线程退出了，" << endl;

    // int* p = new int(10);
    // return (void*)p;

    pthread_exit((void*)1111);
}
int main()
{
    pthread_t tid;
  
    int n = pthread_create(&tid, nullptr, startRoutine, (void*)"thread 1");

    // (void)n;
    

     
    // pthread_cancel(tid);
    // cout << "新线程被取消" << endl;
    

    // while (true)
    // {
    //     printTid("主线程", pthread_self());
    //     // cout << "主线程在运行..." << endl;
    //     sleep(1);
        
    // }
 

    void* ret;
    pthread_join(tid, &ret);//void** retval是一个输出型参数
    printf("%d",(int*)ret );
    // cout << "主线程等待成功, *ret =" << (char*)ret << endl;
    //  delete (int*)ret;
    return 0;
}

// #include <stdio.h>
// #include <pthread.h>

// // 线程要执行的函数，arg用来接收线程传递过来的数据
// void *ThreadFun(void *arg)
// {
//     // 终止线程的执行，将"http://c.biancheng.net"返回
//     pthread_exit((void*)"http://c.biancheng.net");
//     printf("*****************"); // 此语句不会被线程执行
// }

// int main()
// {
//     int res;
//     // 创建一个空指针
//     void *thread_result;
//     // 定义一个表示线程的变量
//     pthread_t myThread;

//     res = pthread_create(&myThread, NULL, ThreadFun, NULL);
//     if (res != 0) {
//         printf("线程创建失败");
//         return 0;
//     }
//     // 等待myThread线程执行完成，并用thread_result指针接收该线程的返回值
//     res = pthread_join(myThread, &thread_result);
//     if (res != 0) {
//         printf("等待线程失败");
//     }
//     printf("%s\n", (char *)thread_result);
//     return 0;
// }