#include <iostream>
#include <queue>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

using namespace std;
const uint32_t gDefaultCap = 5;

template <class T>
class BlockQueue
{
public:
    BlockQueue(uint32_t cap = gDefaultCap)
        :cap_(cap)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&conCond_, nullptr);
        pthread_cond_init(&proCond_, nullptr);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&conCond_);
        pthread_cond_destroy(&proCond_);
    }

public:
    //生产接口
    void push(const T& in) 
    {
        lockQueue();//加锁
        while(isFull())
        {
           proBlockWait();//等待条件满足 被唤醒
           //醒来是在临界区醒来的
            
        }

        //条件满足可以生产
        pushCore(in);//生产完成

        unlockQueue();//解锁

        wakeupCon();//唤醒消费者
    }

    //消费接口
    T pop()
    {
        lockQueue();//加锁
        while(isEmpty())
        {
            conBlockwait();
        }

        //条件满足可以消费
        T tmp = popCore();
        unlockQueue();//解锁

        //唤醒生产者
        wakeupPro();

        return tmp;//返回拿出的数据

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
    bool isEmpty()
    {
        return bq_.empty();
    }
    bool isFull()
    {
        return bq_.size() == cap_;
    }


    void proBlockWait()//生产者一定在临界区中
    {
        pthread_cond_wait(&proCond_, &mutex_);
    }

    //等待被唤醒
    void conBlockwait()
    {
        //1.在阻塞线程的时候，会自动释放mutex_锁
        pthread_cond_wait(&conCond_, &mutex_);
        //2.
    }

    void wakeupPro()//唤醒生产者
    {
        pthread_cond_signal(&proCond_);
    }
    void wakeupCon()//唤醒消费者
    {
        pthread_cond_signal(&conCond_);
    }

    void pushCore(const T& in)
    {
        bq_.push(in);//生产完成
    }

    T popCore()
    {
        T tmp = bq_.front();
        bq_.pop();
        return tmp;
    }
    
private:
    uint32_t cap_;          //容量
    queue<T> bq_;           //阻塞队列
    pthread_mutex_t mutex_; //保护阻塞队列的互斥锁
    pthread_cond_t conCond_; //让消费者等待的条件变量
    pthread_cond_t proCond_; //让生产者等待的条件变量
};