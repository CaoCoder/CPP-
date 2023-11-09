#pragma once

#include <iostream>
#include <cassert>
#include <queue>
#include <memory>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>
#include "Lock.hpp"


using namespace std;

int gThreadNum = 15;

template <class T>
class ThreadPool
{
private:
    ThreadPool(int threadNum = gThreadNum)
        :threadNum_(threadNum)
        ,isStart_(false)
    {
        assert(threadNum_ > 0);
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }
    ThreadPool(const ThreadPool<T> &) = delete;
    void operator=(const ThreadPool<T>&) = delete;


public:
    static ThreadPool<T>* getInstance()
    {
        static Mutex mutex;
        if(nullptr == instance)
        {
            LockGuard lockguard(&mutex);//进入代码块加锁，退出代码块直接解锁 另个花括号之间就是作用域
            if(nullptr == instance)
            {
                instance = new ThreadPool<T>();
            }
            
        }
        cout << "getInstance成功" <<endl;
        fflush(stdout);
        return instance;
    }

    //类内成员，成员函数默认有this
    static void* threadRoutine(void* args)
    {
        pthread_detach(pthread_self());
        ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
        while(1)
        {
            tp->lockQueue();//加锁
            while (!tp->haveTask())//判断队列是否有任务
            { 
                tp->waitForTask();//没有任务 就在条件变量下去等待(会自动释放锁，因为其他地方还需要进入临界区，比如push函数)
            }
            T t = tp->pop();//拿数据到线程上下文
            tp->unlockQueue();//解锁
            t();//处理任务
        }
    }

    void start()
    {
        assert(!isStart_);
        //创建threadNum_个线程
        for (int i = 0; i < threadNum_; i++)
        {
            pthread_t temp;
            pthread_create(&temp, nullptr, threadRoutine, this);
        }
        isStart_ = true;//只运行一次？
        cout << "start成功1" << endl;
    }
    void push(const T& in)
    {
        lockQueue();//加锁
        taskQueue_.push(in);
        choiceThreadForHandler();//生产一个数据 就 唤醒一个线程
        unlockQueue();
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
    int threadNum()
    {
        return threadNum_;
    }
private:
    void lockQueue()
    {
        pthread_mutex_lock(&mutex_);
    }
    void unlockQueue()
    {
        pthread_mutex_unlock(&mutex_);
    }

    bool haveTask()
    {
        return !taskQueue_.empty();
    }
    /*
        pthread_cond_wait() 的调用流程如下：

    1.当前线程调用 pthread_cond_wait(&cond_, &mutex_) 后，它会进入阻塞状态，等待条件变量的信号。
    2.在等待期间，pthread_cond_wait() 会自动释放 &mutex_ 所指向的互斥锁，允许其他线程进入临界区。
    3.当某个线程满足了条件并调用 pthread_cond_signal() 或 pthread_cond_broadcast() 发送信号时，等待的线程会被唤醒。
    4.一旦被唤醒，pthread_cond_wait() 函数会重新获取 &mutex_ 所指向的互斥锁，并继续执行后续的代码。

   需要注意的是，在调用 pthread_cond_wait() 之前，必须先获取 &mutex_ 所指向的互斥锁，以确保没有竞争条件。因此，通常会按照如下方式使用条件变量：

    1.获取互斥锁 &mutex_。
    2.检查条件是否满足，如果不满足，则调用 pthread_cond_wait(&cond_, &mutex_) 进行等待。
    3.当条件满足时，执行相应的操作。
    4.发送信号或广播通知其他线程条件已满足。
    5.释放互斥锁 &mutex_。

    */
    void waitForTask()
    {
        pthread_cond_wait(&cond_, &mutex_);
    }

    void choiceThreadForHandler()
    {
        pthread_cond_signal(&cond_);
    }

    T pop()
    {
        T temp = taskQueue_.front();
        taskQueue_.pop();
        return temp;
    }
private:
    bool isStart_;
    int threadNum_;
    queue<T> taskQueue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    //类内声明
    static ThreadPool<T>* instance;
};

//类外定义初始化
template <class T>
ThreadPool<T>* ThreadPool<T>::instance = nullptr;