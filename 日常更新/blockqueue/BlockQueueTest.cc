#include "BlockQueue.hpp"
#include "Task.hpp"
#include "stdio.h"
#include "ctime"

const std::string ops = "+-*/%";

void* consumer(void* args)
{
    BlockQueue<Task>* bpq = static_cast<BlockQueue<Task>*>(args);
    while(true)
    {
        //消费任务
        Task t = bpq->pop();
        //处理任务
        int result = t();
        int one, two;
        char op;
        t.get(&one, &two, &op);
        cout << "消费者：" << pthread_self() << (unsigned long)time(nullptr) << " 消费了一个任务: " << one << op << two << "=" << result << endl;
        sleep(1);

    }
}

void* productor(void* args)
{
    BlockQueue<Task>* bqp = static_cast<BlockQueue<Task>*>(args);
    while(true)
    {
        //制作任务
        int one = rand() % 50;
        int two = rand() % 50;
        char op = ops[rand() %  ops.size()];
        //生产任务
        Task t(one, two, op);
        bqp->push(t);
        cout << "生产者：" << pthread_self() << (unsigned long)time(nullptr) << " 生产了一个任务: " << one << op << two << "=?" << endl;
        
    }                         
}
int main()
{
    srand((unsigned long)time(nullptr) ^ getpid() );

    BlockQueue<Task> bq;

    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, &bq);
    pthread_create(&p, nullptr, productor, &bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}