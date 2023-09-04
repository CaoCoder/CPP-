#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>

#define NUM 1024

#define NONE_FLUSH 0x0
#define LINE_FLUSH 0x1
#define FULL_FLUSH 0x2

typedef struct _MyFILE
{
  int _fileno;
  char _buffer[NUM];
  int _end;
  int _flags;//刷新措施
}MyFILE;

MyFILE* my_fopen(const char* filename, const char* method)
{
  assert(filename);
  assert(method);

  int flags = O_RDONLY;
  if(strcmp(method, "w") == 0)
  {
    flags = O_WRONLY | O_CREAT | O_TRUNC;
  }
  else if(strcmp(method, "a")== 0)
  {
    flags = O_WRONLY | O_CREAT | O_APPEND;
  }
  
  int fileno = open(filename, flags, 0666);
  if(fileno < 0)
  {
    return NULL;
  }
  MyFILE* fp = (MyFILE*)malloc(sizeof(MyFILE));
  if(fp == NULL)
    return fp;
  memset(fp, 0,sizeof(MyFILE));
  fp->_fileno = fileno;
  fp->_flags |= LINE_FLUSH;
  fp->_end = 0;
  return fp;

}
void my_fflush(MyFILE* fp)
{
  if(fp->_end > 0)
  {
    write(fp->_fileno , fp->_buffer, fp->_end);
    fp->_end = 0;
    syncfs(fp->_fileno);
  }
}
void my_fwrite(MyFILE* fp, const char* start, int len)
{
  assert(fp);
  assert(start);
  assert(len > 0);
  //写到缓冲区里
  strncpy(fp->_buffer+fp->_end, start, len);
  fp->_end += len;

  if(fp->_flags & NONE_FLUSH)
  {}
  else if(fp->_flags & LINE_FLUSH)
  {
    if(fp->_end > 0 && fp->_buffer[fp->_end-1] == '\n')
    {
        write(fp->_fileno, fp->_buffer, fp->_end);
        fp->_end = 0;
        syncfs(fp->_fileno);
    }
  }

}
my_fclose(MyFILE* fp)
{
  my_fflush(fp);
  close(fp->_fileno);
  free(fp);
}
int main()
{
  MyFILE* fp = my_fopen("log.txt", "a");
  if(fp == NULL)
  {
    printf("my_fopen error\n");
    return 1;
  }
  //const char* s = "hello my 111\n";
  //
  //my_fwrite(fp, s, strlen(s));
  //printf("消息立即刷新");
  
  //const char* ss = "hello my 222";
  //my_fwrite(fp, ss, strlen(ss));
  //printf("不满足\n");
  //binsleep(3);
  const char* s = "-aaaaaa";
  my_fwrite(fp, s, strlen(s));
  printf("不满足\n");
  

  my_fclose(fp);
}
