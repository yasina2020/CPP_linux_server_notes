/*
#include <unistd.h>
int execl(const char *path,const char *arg,...);
- path:需要指定的执行的文件的（绝对）路径
- arg:是执行文件所需要的参数列表 可以是很多个参数    参考main函数
  参数以null结束
返回值：只有调用出错时返回-1设置errno


*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("1我是子进程，pid是%d，我的父进程是%d\n",getpid(),getppid());
        execl("/home/fang/newcodercpp/lesson03/hello","hello",NULL);
        printf("2我是子进程，pid是%d，我的父进程是%d\n",getpid(),getppid());
    }else if(pid > 0){
        printf("我是main进程，pid是%d\n",getpid());
        sleep(1);
    }

    for(int i=0;i<3;i++){
        printf("i=%d,pid是%d\n",i,getpid());
    }

}