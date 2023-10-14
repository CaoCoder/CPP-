#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unistd.h>
#include <pthread.h>

using namespace std;

//定义一个条件变量
pthread_cond_t cond;
//定义一个互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vector<function<void()>> funcs;


void show()
{
    cout << "hello show" << endl;
}

void print()
{
    cout << "hello print" << endl;
}

//定义全局退出变量

volatile bool quit  = false;

void* waitCommand(void* args)
{
    //  pthread_detach(pthread_self());
    while(!quit)
    {
        pthread_cond_wait(&cond, &mutex);
        for(auto& f : funcs)
        {
            f();
        }
        cout << "thread id: " << pthread_self() << " end..." << endl;

    }

    return nullptr;
}

int main()
{
    funcs.push_back(show);
    funcs.push_back(print);
    funcs.push_back([](){
        cout << "你好世界！" << endl;
    });

    pthread_cond_init(&cond, nullptr);
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, waitCommand, nullptr);
    pthread_create(&t2, nullptr, waitCommand, nullptr);
    pthread_create(&t3, nullptr, waitCommand, nullptr);

    while(true)
    {
        sleep(1);
        //  pthread_cond_broadcast(&cond);

        char n = 'a';
        cout << "请输入你的command(n/q): ";
        cin >> n;
        // if(n == 'n')
        //      pthread_cond_signal(&cond);
        // else break;
        if(n == 'n') 
        {
            // quit = true;
            cout << "quit : " << quit << endl;
            // sleep(1);

            pthread_cond_signal(&cond);
            sleep(1);
            // break;
        }
        else
        {
            quit = true;
            break;
        }

        // sleep(1);

        // pthread_cond_signal(&cond);
    }

    cout << "主线程 停止" << endl;

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);

    pthread_cond_destroy(&cond);
    return 0;
}