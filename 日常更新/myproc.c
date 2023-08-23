#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //int cnt = 5;
        //child
        while(1)
        {
            printf("我是子进程, 我正在运行...Pid: %d\n", getpid());
            sleep(5);
            break;
            //int a = 10/0;
          //  cnt--;
          //  if(!cnt)
          //  {
          //      break;
          //  }
        }
        exit(10);
    }
    else
    {
        int status = 0;
        printf("我是父进程：pid:%d, 我准备电脑等待子进程啦\n", getpid());
        pid_t ret = waitpid(id, &status, 0);
        if(ret > 0)
        {
            if(WIFEXITED(status))
            {
                printf("子进程是正常退出的，退出码： %d\n", WEXITSTATUS(status));
            }
            //code??可以吗？？
            //printf("wait success, ret : %d, 我所等待的子进程的退出码: %d, 退出信号是: %d\n", 
            //        ret, (status>>8)&0xFF, status&0x7F); //status >>= 8;
        }
     //   sleep(40);
     //   pid_t ret = wait(NULL);
     //   if(ret < 0)
     //   {
     //       printf("等待失败!\n");
     //   }
     //   else
     //   {
     //       printf("等待成功: result: %d\n", ret);
     //   }

     //   sleep(20);
    }
}



















//int cnt = 0;
//
//void fun()
//{
//    printf("hello fun!\n");
//    _exit(111);
//}
//
//
//int main()
//{
//    //_exit(222);
//    //fun();
//    //printf("hello bit");
//    //sleep(1);
//    //_exit(111);
//    //return 10;
// //   for(int i = 0; i < 100; i++)
// //   {
// //       printf("%d: %s\n", i, strerror(i));
// //   }
////    for(;;)
////    {
////        pid_t id = fork();
////        if(id < 0)
////        {
////            printf("创建子进程失败....:%d\n", cnt);
////            break;
////        }
////        if(id == 0)
////        {
////            printf("I am a child... %d\n", getpid());
////            sleep(20);
////            exit(0);
////        }
////        cnt++;
////    }
////
//////    printf("我是一个进程: pid: %d\n", getpid());
////    fork();
////    printf("我依旧是一个进程: pid:%d\n", getpid());
////
////    sleep(1);
////    pid_t id = fork();
////    if( id == 0 )
////    {
////        while(1)
////        {
////            printf("我是子进程,pid: %d, ppid: %d\n", getpid(), getppid());
////            sleep(1);
////        }
////    }
////    else
////    {
////        while(1)
////        {
////            printf("我是父进程,pid: %d, ppid: %d\n", getpid(), getppid());
////            sleep(1);
////        }
////    }
//}
