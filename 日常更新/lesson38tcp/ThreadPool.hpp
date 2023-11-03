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
            LockGuard lockguard(&mutex);//进入代码块加锁，退出代码块直接解锁
            if(nullptr == instance)
            {
                instance = new ThreadPool<T>();
            }
            
        }

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
                tp->waitForTask();//在条件变量下去等待
            }
            T t = tp->pop();//拿数据到线程上下文
            tp->unlockQueue();//解锁
            t();//
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