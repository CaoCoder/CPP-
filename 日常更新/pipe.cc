#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>

using namespace std;

typedef void (*functor)();

vector<functor> functors; // 方法集合
// for debug
unordered_map<uint32_t, string> info;

void f1()
{
    cout << "这是一个处理日志的任务, 执行的进程 ID [" << getpid() << "]"
         << "执行时间是[" << time(nullptr) << "]\n" << endl;
    //
}
void f2()
{
    cout << "这是一个备份数据任务, 执行的进程 ID [" << getpid() << "]"
         << "执行时间是[" << time(nullptr) << "]\n" << endl;
}
void f3()
{
    cout << "这是一个处理网络连接的任务, 执行的进程 ID [" << getpid() << "]"
         << "执行时间是[" << time(nullptr) << "]\n" << endl;
}

void loadFunctor()
{
    info.insert({functors.size(), "处理日志的任务"});
    functors.push_back(f1);

    info.insert({functors.size(), "备份数据任务"});
    functors.push_back(f2);

    info.insert({functors.size(), "处理网络连接的任务"});
    functors.push_back(f3);
}

// int32_t: 进程pid, int32_t: 该进程对应的管道写端fd
typedef std::pair<int32_t, int32_t> elem;
int processNum = 5;

void work(int blockFd)
{
    cout << "进程[" << getpid() << "]" << " 开始工作" << endl;
    // 子进程核心工作的代码
    while (true)
    {
        // a.阻塞等待  b. 获取任务信息
        uint32_t operatorCode = 0;
        ssize_t s = read(blockFd, &operatorCode, sizeof(uint32_t));
        if(s == 0) break;
        assert(s == sizeof(uint32_t));
        (void)s;

        // c. 处理任务
        if(operatorCode < functors.size()) functors[operatorCode]();
    }
    cout << "进程[" << getpid() << "]" << " 结束工作" << endl;
}

// [子进程的pid, 子进程的管道fd]
void blanceSendTask(const vector<elem> &processFds)
{
    srand((long long)time(nullptr));
    while(true)
    {
        sleep(1);
        // 选择一个进程, 选择进程是随机的，没有压着一个进程给任务
        // 较为均匀的将任务给所有的子进程 --- 负载均衡
        uint32_t pick = rand() % processFds.size();

        // 选择一个任务
        uint32_t task = rand() % functors.size();

        // 把任务给一个指定的进程
        write(processFds[pick].second, &task, sizeof(task));

        // 打印对应的提示信息
        cout << "父进程指派任务->" << info[task] << "给进程: " << processFds[pick].first << " 编号: " << pick << endl;
    }
}

int main()
{
    loadFunctor();
    vector<elem> assignMap;
    // 创建processNum个进程
    for (int i = 0; i < processNum; i++)
    {
        // 定义保存管道fd的对象
        int pipefd[2] = {0};
        // 创建管道
        pipe(pipefd);
        // 创建子进程
        pid_t id = fork();
        if (id == 0)
        {
            // 子进程读取, r -> pipefd[0]
            close(pipefd[1]);
            // 子进程执行
            work(pipefd[0]);
            close(pipefd[0]);
            exit(0);
        }
        //父进程做的事情, pipefd[1]
        close(pipefd[0]);
        elem e(id, pipefd[1]);
        assignMap.push_back(e);
    }
    cout << "create all process success!" << std::endl;
    // 父进程, 派发任务
    blanceSendTask(assignMap);

    // 回收资源
    for (int i = 0; i < processNum; i++)
    {
        if (waitpid(assignMap[i].first, nullptr, 0) > 0)
            cout << "wait for: pid=" << assignMap[i].first << " wait success!"
                 << "number: " << i << "\n";
        close(assignMap[i].second);
    }
}

// 2. 父进程控制子进程
// int main()
// {
//     // 0. 加载任务列表
//     loadFunctor();

//     // 1. 创建管道
//     int pipefd[2] = {0};
//     if (pipe(pipefd) != 0)
//     {
//         cerr << "pipe error" << endl;
//         return 1;
//     }

//     // 2. 创建子进程
//     pid_t id = fork();
//     if (id < 0)
//     {
//         cerr << " fork error " << endl;
//         return 2;
//     }
//     else if (id == 0)
//     {
//         // 3. 关闭不需要的文件fd
//         // child,read
//         close(pipefd[1]);
//         // 4. 业务处理
//         while (true)
//         {
//             uint32_t operatorType = 0;
//             // 如果有数据，就读取，如果没有数据，就阻塞等待, 等待任务的到来
//             ssize_t s = read(pipefd[0], &operatorType, sizeof(uint32_t));
//             if (s == 0)
//             {
//                 cout << "我要退出啦,我是给人打工的,老板都走了...." << endl;
//                 break;
//             }
//             assert(s == sizeof(uint32_t));
//             // assert断言，是编译有效 debug 模式
//             // release 模式，断言就没有了.
//             // 一旦断言没有了，s变量就是只被定义了，没有被使用。release模式中，可能会有warning
//             (void)s;

//             if (operatorType < functors.size())
//             {
//                 functors[operatorType]();
//             }
//             else
//             {
//                 cerr << "bug? operatorType = " << operatorType << std::endl;
//             }
//         }
//         close(pipefd[0]);
//         exit(0);
//     }
//     else
//     {
//         srand((long long)time(nullptr));
//         // parent,write - 操作
//         // 3. 关闭不需要的文件fd
//         close(pipefd[0]);
//         // 4. 指派任务
//         int num = functors.size();
//         int cnt = 10;
//         while (cnt--)
//         {
//             // 5. 形成任务码
//             uint32_t commandCode = rand() % num;
//             std::cout << "父进程指派任务完成，任务是: " << info[commandCode] << " 任务的编号是: " << cnt << std::endl;
//             // 向指定的进程下达执行任务的操作
//             write(pipefd[1], &commandCode, sizeof(uint32_t));
//             sleep(1);
//         }
//         close(pipefd[1]);
//         pid_t res = waitpid(id, nullptr, 0);
//         if (res)
//             cout << "wait success" << endl;
//     }
//     return 0;
// }

// 演示pipe通信的基本过程 -- 匿名管道
// int main()
// {
//     // 1. 创建管道
//     int pipefd[2] = {0};
//     if(pipe(pipefd) != 0)
//     {
//         cerr << "pipe error" << endl;
//         return 1;
//     }
//     // 2. 创建子进程
//     pid_t id = fork();
//     if(id < 0)
//     {
//         cerr << "fork error" << endl;
//         return 2;
//     }
//     else if (id == 0)
//     {
//         // child
//         // 子进程来进行读取, 子进程就应该关掉写端
//         close(pipefd[1]);
//         #define NUM 1024
//         char buffer[NUM];
//         while(true)
//         {
//             cout << "时间戳: " << (uint64_t)time(nullptr) << endl;
//             // 子进程没有带sleep，为什么子进程你也会休眠呢？？
//             memset(buffer, 0, sizeof(buffer));
//             sleep(100);
//             ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
//             if(s > 0)
//             {
//                 //读取成功
//                 buffer[s] = '\0';
//                 cout << "子进程收到消息,内容是: " << buffer << endl;
//             }
//             else if(s == 0)
//             {
//                 cout << "父进程写完了，我也退出啦" << endl;
//                 break;
//             }
//             else{
//                 //Do Nothing
//             }
//         }
//         close(pipefd[0]);
//         exit(0);
//     }
//     else
//     {
//         // parent
//         // 父进程来进行写入，就应该关掉读端
//         close(pipefd[0]);
//         const char *msg = "你好子进程，我是父进程, 这次发送的信息编号是: ";
//         int cnt = 0;
//         // while(cnt < 5)
//         while(1)
//         {
//             char sendBuffer[1024];
//             sprintf(sendBuffer, "%s : %d", msg, cnt);
//             //sleep(30); // 这里是为了一会看现象明显
//             write(pipefd[1], sendBuffer, strlen(sendBuffer)); //要不要+1 1,0
//             cnt++;
//             cout << "cnt: " << cnt << endl;
//         }
//         close(pipefd[1]);
//         cout << "父进程写完了" << endl;
//     }
//     pid_t res = waitpid(id, nullptr, 0);
//     if(res > 0)
//     {
//         cout << "等待子进程成功" << endl;
//     }
//     // 0 -> 嘴巴 -> 读（读书）
//     // 1 -> 笔 -> 写的
//     // cout << "fd[0]: " << pipefd[0] << endl;
//     // cout << "fd[1]: " << pipefd[1] << endl;
//     return 0;
// }
