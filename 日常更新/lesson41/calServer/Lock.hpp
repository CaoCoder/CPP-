#pragma once
#include <iostream>
#include <pthread.h>
class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&lock_, nullptr);
    }

    void lock()
    {
        pthread_mutex_lock(&lock_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&lock_);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&lock_);
    }
private:
    pthread_mutex_t lock_;
};

class LockGuard
{
public:
    LockGuard(Mutex* mutex)
        :mutex_(mutex)
        {
            mutex_->lock();
            std::cout << " 加锁成功了...." << std::endl;
        }
    ~LockGuard()
    {
        std::cout << " 解锁成功了...." << std::endl;
    }
private:
    Mutex* mutex_;
};