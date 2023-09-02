#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SEP " "
#define NUM 1024
#define SIZE 128

char command_line[NUM];
char* command_args[SIZE];

//对应上层的内建命令
int ChangeDir(const char * new_path)
{
    chdir(new_path);//系统调用

    return 0; // 调用成功
}

int main()
{
    //shell本质是一个死循环
    while(1)
    {
        //1.显示提示符
        printf("[张三@我的主机名 当前目录]# ");
        fflush(stdout);

        //2.获取用户输入
        memset(command_line,'\0',sizeof(command_line)*sizeof(char));
        fgets(command_line,NUM,stdin);//可以得到'\n'
        command_line[strlen(command_line)-1] = '\0';//去除换行符'\n'

        //3.字符串切分"ls -a -l -i" -> "ls" "-a" "-l" "-i"
        command_args[0] = strtok(command_line,SEP);
        int index = 1;

        //给ls命令添加颜色
        if(strcmp(command_args[0], "ls") == 0)
        {
            command_args[index++] = (char*)"--color=auto";
        }

        while(command_args[index++] = strtok(NULL,SEP));

        // 4. TODO, 编写后面的逻辑, 内建命令
        if(strcmp(command_args[0], "cd") == 0 && command_args[1] != NULL)
        {
            ChangeDir(command_args[1]); //让调用方进行路径切换, 父进程
            continue;
        }

        //5.创建进程，执行

        pid_t id  = fork();
        if(id == 0)
        {
            execvp(command_args[0],command_args);
            exit(1);
        }
        int status = 0;
        pid_t ret = waitpid(id, &status,0);
        if(ret > 0)
        {
            printf("等待子进程成功：sig: %d, code: %d\n", status&0x7F, status>>8&(0xFF));
        }


    }

       



    return 0;
}

