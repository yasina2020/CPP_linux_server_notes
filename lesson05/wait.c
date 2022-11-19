/*
#inlcude <sys/wait.h>

pid_t wait(int *wstatus);
    等待任意一个子进程结束，如果任意一个子进程结束，次函数会回收
    参数：
        int *wstatus
            进程退出时的状态信息会被记录，这是一个传出参数。
    返回值：
        成功返回被回收子进程的pid
        失败（所有的子进程都结束，或者调用失败）返回-1
    调用wait()函数的进程会被阻塞挂起，直到有一个子进程退出，或收到一个不能被忽略的信号。


*/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid;
    for(int i=0;i<5;i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }
    if(pid > 0){
        // 父进程
        while(1){
            printf("父进程的%d\n",getpid());
            pid_t ret = wait(0);
            if(ret != -1){
                printf("子进程结束-资源已回收--子进程的PID是%d\n",ret);
            }else{
                printf("子进程全部结束-资源已全部回收\n");
            }
            
            sleep(1);
        }

    }else if (pid == 0)
    {
        // 子进程
        printf("子进程的%d\n",getpid());
    }
    
}