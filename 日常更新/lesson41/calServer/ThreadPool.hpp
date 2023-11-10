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

/*

在这个代码片段中，线程池实例 instance 被声明为静态指针，
并且初始值为 nullptr。在第一个 if 条件判断中，检查 instance 是否为 nullptr，
如果是，则进入临界区并创建线程池对象。

懒汉模式的主要特点是延迟加载，即在第一次使用时才创建对象
*/
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
    
    ThreadPool(const ThreadPool<T>& ) = delete;
    void operator=(const ThreadPool<T>& ) = delete;
public:
    /*
    如果不使用 static 关键字声明 mutex，而是在每次函数调用时创建一个局部变量 mutex，则每次函数调用都会创建一个新的 mutex 对象。这样会导致以下问题：

    1.无法实现线程同步：每次函数调用都会创建一个新的 mutex 对象，无法保证多个线程之间共享同一个互斥锁。这样就无法实现线程同步，多个线程可能会同时进入临界区，导致竞态条件和错误的结果。

    2.重复创建互斥锁：每次函数调用都会创建一个新的 mutex 对象，这样会导致重复创建多个互斥锁，占用不必要的系统资源。

    */
    

   /*
        函数使用static的原因：
        1.不需要依赖具体的对象实例，静态函数可以在没有对象实例的情况下直接调用，不需要通过对象来访问
        2.提供统一的访问接口，静态函数可以通过类名直接调用，无需创建对象实例。这样可以提供一个统一的访问接口
        3.全局可访问性
   */

  /*临界区是以下代码块
    LockGuard Lockguard(&mutex);

    if (nullptr == instance)
    {
        instance = new ThreadPool<T>();
    }
  */
    static ThreadPool<T>* getInstance()
    {
        static Mutex mutex;//只会初始化一次
        if(nullptr == instance)//第一个if是为了只产生一个实例线程池对象
        {
            LockGuard Lockguard(&mutex);
            //第二个if是为了避免几个线程同时通过了第一个if的判断，都在这里等待。
            //重点是在获取锁之前就已经在这里等待了，
            //但是这个对象已经被第一个拿到锁的线程创建了!!。
            //所以在这等待的线程也会拿到锁，然后进入临界区，进行线程池对象的创建，所以为了避免再次创建
            //这里就要再用一次if来进行判断。
            if(nullptr == instance)
            {
                instance = new ThreadPool<T>();
            }
        }
        
        return instance;
    }
    /*
    线程的分离状态是指线程的执行完成后，
    系统会自动回收线程的资源，而不需要显式调用 pthread_join 来等待线程退出。
    为了避免僵尸线程
    */
        //类内成员, 成员函数，都有默认参数this
    static void *threadRoutine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadPool<T> *tp = static_cast<ThreadPool<T> *>(args);
        // prctl(PR_SET_NAME, "follower"); // 更改线程名称
        while (1)
        {
            tp->lockQueue();
            while (!tp->haveTask())
            {
                //没有任务 就在条件变量下去等待(会自动释放锁，因为其他地方还需要进入临界区，比如push函数)
                tp->waitForTask();
            }
            //这个任务就被拿到了线程的上下文中
            T t = tp->pop();
            tp->unlockQueue();
            t(); // 让指定的先处理这个任务
        }
    }

    void start()
    {
        assert(!isStart_);
        for (int i = 0; i < threadNum_; i++)
        {
            pthread_t temp;
            pthread_create(&temp, nullptr, threadRoutine, this);
        }
        isStart_ = true;
    }
    void push(const T &in)
    {
        lockQueue();
        taskQueue_.push(in);
        choiceThreadForHandler();
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
     void lockQueue() { pthread_mutex_lock(&mutex_); }
      void unlockQueue() { pthread_mutex_unlock(&mutex_); }
     bool haveTask() { return !taskQueue_.empty(); }
    void waitForTask() { pthread_cond_wait(&cond_, &mutex_); }
    void choiceThreadForHandler() { pthread_cond_signal(&cond_); }
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

    static ThreadPool<T>* instance;

};

template<class T>
ThreadPool<T>* ThreadPool<T>::instance = nullptr;//加不加类域的区别