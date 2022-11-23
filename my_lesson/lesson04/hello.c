#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
    printf("Hello World!\n");
    printf("3我是子进程，pid是%d，我的父进程是%d\n",getpid(),getppid());
    return 0;
}