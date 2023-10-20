#include "ThreadPool.hpp"
#include "Task.hpp"
#include <ctime>
#include <thread>

int main()
{
    //PR_SET_NAME 表示设置进程名称 第二个参数是进程的名称。
    prctl(PR_SET_NAME, "master");

    const string operators = "+/*/%";
    // 可以确保线程池对象在不再使用时会被自动销毁和释放
    unique_ptr<ThreadPool<Task>> tp(ThreadPool<Task>::getInstance());//创建线程池
    tp->start();//创建 一堆 线程

    //根据当前时间、进程ID和线程ID初始化随机数生成器。
    srand((unsigned long)time(nullptr) ^ getpid() ^ pthread_self());
    //派发任务的主线程
    while(true)
    {   
        //制作任务
        int one = rand() % 50;
        int two = rand() % 10;
        char oper = operators[rand() % operators.size()];
        Log() << "主线程派发计算任务：" << one << oper << two << "=?" << "\n";
        Task t(one, two, oper);

        tp->push(t);//加入任务,并唤醒线程
        sleep(1);
    }
}