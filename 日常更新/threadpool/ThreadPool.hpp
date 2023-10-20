#pragma

#include <iostream>
#include <cassert>
#include <queue>
#include <memory>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>
#include "Lock.hpp"
#include "Log.hpp"

using namespace std;

int gThreadNum = 5;

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

    ThreadPool(const ThreadPool<T>&) = delete;
    void operator=(const ThreadPool<T>&) = delete;
    
public:
    static ThreadPool<T>* getInstance()
    {
        static Mutex mutex;
        if(nullptr == instance)//仅仅是过滤重复的判断
        {
            LockGuard lockguard(&mutex);//加锁，退出代码块，自动解锁
            if(nullptr == instance)
            {
                //创建线程池
                instance = new ThreadPool<T>();
            }
        }
        return instance;
    }

    //类内成员，隐藏this
    static void* threadRoutine(void* args)
    {
        //线程分离，线程终止自动释放资源，而无需其他线程调用 pthread_join 来等待它的终止。
        pthread_detach(pthread_self());

        ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
        prctl(PR_SET_NAME, "follower");
        while(1)
        {
            tp->lockQueue();//对任务队列加锁
            while(!tp->haveTask())//任务队列是否为空
            {
                tp->waitForTask();//为空则在条件变量下等待，自动释放锁，在临界区醒来，自动拿到锁
            }

            //这个任务就被拿到了线程的上下文中
            T t = tp->pop();//把任务拿给线程
            tp->unlockQueue();//解锁

            //处理任务（处理任务不在临界区，其他线程也可以切换，但是队列没有任务，还需要等待主线程生产任务）
            int one, two;
            char oper;
            t.get(&one, &two, &oper);
            //每个任务都必须有一个run方法
            Log() << "新线程完成计算任务：" << one << oper << two<< "="<< t.run() << "\n";
        
            
        }
    }
    void start()
    {
        assert(!isStart_);
        for(int i = 0; i < threadNum_; i++)
        {
             pthread_t temp;
             pthread_create(&temp, nullptr, threadRoutine, this);
        }
        isStart_ = true;
    }
    
    void push(const T& in)
    {
        lockQueue();//加锁
        taskQueue_.push(in);//向任务队列加数据
        choiceThreadForHandler();//选择唤醒一个线程
        unlockQueue();//解锁
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
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
    
    static ThreadPool<T>* instance;
};

template<class T>
ThreadPool<T>* ThreadPool<T>::instance = nullptr;

