/*
#include <unistd.h>

int execl(const char *path,const char *arg,...);
- path:需要指定的执行的文件的（绝对）路径
- arg:是执行文件所需要的参数列表 可以是很多个参数    参考main函数
  参数以null结束
返回值：只有调用出错时返回-1设置errno

int execlp(const char *file,const char *arg,...);
会到环境变量中查找file，找到就执行
- file 可执行文件的文件名
- arg 

int execv(const char *file,char *const argv[]);
-argv[] 参数列表数组
如 char *argv[] = {"ls","参数1","参数2","参数3"...}
execv("ls",argv[]);

int execve(const char *path,char *const argv[], char *const envp[]);
自定义一个环境变量的数组,会临时的网系统环境变量中添加进去自己定义的
-envp[] 自定义环境变量列表数组
如 char *envp[] = {"PATH=/home/fang/01","PATH=/home/fang/02"...,NULL}


*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("1我是子进程，pid是%d，我的父进程是%d\n",getpid(),getppid());
        execlp("ls","ls","-all",NULL);
        printf("2我是子进程，pid是%d，我的父进程是%d\n",getpid(),getppid());
    }else if(pid > 0){
        printf("我是main进程，pid是%d\n",getpid());
        sleep(1);
    }

    for(int i=0;i<3;i++){
        printf("i=%d,pid是%d\n",i,getpid());
    }

}