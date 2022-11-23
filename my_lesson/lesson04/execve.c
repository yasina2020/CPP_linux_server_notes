/*
    int execve(const char *filename,char *const argv[], char *const envp[]);
    自定义一个环境变量的数组,会临时的网系统环境变量中添加进去自己定义的
    - filename 字符串所代表的文件路径
    - argv[] 利用指针数组来传递给执行文件，并且需要以空指针(NULL)结束
    -envp[] 为传递给执行文件的新环境变量数组
    如 char *envp[] = {"PATH=/home/fang/01","PATH=/home/fang/02"...,NULL}
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();

    if(pid == 0){
        printf("子进程\n");
        char *envp[]={"PATH=/home/fang/newcodercpp/lesson04/","PATH=/home/fang/newcodercpp/lesson03/",NULL};
        char *argv[]={"app",NULL};
        char *filename = "/home/fang/newcodercpp/lesson03/app";
        execve(filename,argv,envp);
    }else if(pid > 0){
        printf("主进程\n");
    }


}