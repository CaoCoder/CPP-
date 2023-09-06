// #include <stdio.h>
// // #include <unistd.h>

// int main(int argc, char* argv[])
// {
//     if(argc != 2)
//     {
//         printf("Usage: <%s> <filename>\n", argv[0]);
//         return 1;
//     }
//     //chdir("/home/csq");更改当前进程的工作路径
//     FILE* fp = fopen(argv[1], "r");
//     if(fp == NULL)
//     {
//         perror("fopen fail");
//         return 1;
//     }

//     char buffer[64];
//     while(fgets(buffer, sizeof(buffer), fp) != NULL)
//     {
//         printf("%s", buffer);
//     }

//     fclose(fp);
// }

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // close(1);
    // //根据fd的分配规则，新的fd值一定是1
    // int fd  = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // if(fd < 0)
    // {
    //     perror("open");
    //     return 1;
    // }

    // // printf->stdout->1->虽然不在指向对应的显示器了，但是已经指向了log.txt的底层struct file对象！
    // printf("fd: %d\n", fd);

    // //为甚必须要fflush? 这个就和我们历史的缓冲区有关了
    // fflush(stdout);

    // close(fd);




















    // 先验证0,1,2就是标准IO
    // char buffer[1024];
    // ssize_t s = read(0, buffer, sizeof(buffer)-1);
    // if(s > 0)
    // {
    //    buffer[s] = '\0';

    //    printf("echo: %s", buffer);
    // }
    // const char *s = "hello write\n";
    // write(1, s, strlen(s));
    // write(2, s, strlen(s));
    // //验证012和stdin，stdout，stderr的对应关系
    // printf("stdin: %d\n", stdin->_fileno);
    // printf("stdout: %d\n", stdout->_fileno);
    // printf("stderr: %d\n", stderr->_fileno);












    umask(0);
    // close(1);
    // fopen("log.txt", "w"); //底层的open，O_WRONLY | O_CREAT | O_TRUNC
    //  int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // fopen("log.txt", "a"); //底层的open，O_WRONLY | O_CREAT | O_APPEND
    // int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    // int fd = open("log.txt", O_RDONLY);
    // close(0);
    // close(1);
    // close(2);
    // int fda = open("loga.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // int fdb = open("logb.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // int fdc = open("logc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // int fdd = open("logd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // int fde = open("loge.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // printf("fda: %d\n", fda);
    // printf("fdb: %d\n", fdb);
    // printf("fdc: %d\n", fdc);
    // printf("fdd: %d\n", fdd);
    // printf("fde: %d\n", fde);
    // close(1);
    int fd;
    char buffer[128];
    ssize_t s = read(fd, buffer, sizeof(buffer)-1);
    printf("%d",fd);
    if(s > 0)
    {
       buffer[s] = '\0';
       printf("%s", buffer);
     
    }
   int cnt = 0;
   //const char *str = "hello file 104\n";
   const char *str = "bbbbbb\n";
   while(cnt < 1)
   {
       write(fd, str, strlen(str));
    printf("%d",fd);

       cnt++;
   }


    close(fd);

    return 0;
}
