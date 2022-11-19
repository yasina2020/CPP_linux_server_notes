/*
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void)
        创建子进程，
        在父进程中返回子进程pid，-1 失败，并设置errno
        如果是0 就是在子进程返回，
        返回两次 一次在父进程，一次在子进程。
        写时拷贝 读时共享

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    // 
    pid_t pid = fork();
// getpid() 获取当前进程pid  getppid() 获得当前进程的父进程的pid
    if(pid > 0){
        //pid>0 在父进程 当cpu切换到父进程时，运行这一段代码
         printf("main %d,%d\n",getpid(),getppid());
    }else if( pid== 0){
        //pid=0 在子进程 当cpu切换到子进程时，运行这一段代码
        printf("child %d,%d\n",getpid(),getppid());
    }

    //这段代码在父子进程中都运行
    for(int i=0;i<3;i++){
        printf("i = %d,getpid=%d\n",i,getpid());
        // sleep(1);
    }
    return 0;
}



