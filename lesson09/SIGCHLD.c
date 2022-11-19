/*
产生条件
    1、子进程终止时，
    2、子进程收到SIGSTOP信号停止时
    3、子进程处在停止态，接收到SIGCONT后唤醒时
以上三种条件都会给父进程发送SIGCHLD信号，父进程默认会忽略该信号。

可以在父进程中捕获该信号解决僵尸进程的问题。

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
// 收到SIGCHLD信号后会调用该函数
void my_call(int signal_number){
    // 利用waitpid回收子进程
    while(1){
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == 0){
            // =0 没有已经结束的子进程了
            printf("没有已经结束的子进程了\n");
            break;
        }else if(ret == -1){
            // 出错了，
            printf("出错了\n");
            break;
        }else if(ret > 0){
            printf("捕捉到信号%d,子进程%d已结束\n",signal_number,ret);
        }
        
    }
}

int main(){
/*
这里有两种解决方案，
第一种是在程序一开始就注册信号事件
第二种是先阻塞SIGCHLD信号
*/ 

#define FLAG 2

#if FLAG==1
    // 第一种是在程序一开始就注册信号事件
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = my_call;
        sigaction(SIGCHLD,&act,NULL);
#else
    // 第二种是先阻塞SIGCHLD信号
    // 为防止子进程先消失，直接答SIGCHLD信号发送完了，设置阻塞
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
#endif
   
    // 创建子进程
    pid_t pid;
    for(int i=0;i<20;i++){
        pid = fork();
        if(pid==0){
            break;
        }
    }
    if(pid > 0){
        #if FLAG!=1
            // 在父进程中设置捕获信号SIGCHLD
            struct sigaction act;
            act.sa_flags = 0;
            act.sa_handler = my_call;
            sigaction(SIGCHLD,&act,NULL);
            // 设置完捕获动作之后就可以解除阻塞了
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        #endif
        // 父进程一直循环。
        while(1){
            sleep(1);
        }
    }else if(pid==0){
        // 子进程打印一句话后结束。这时候父进程会受到SIGCHLD信号
        printf("i am child ,pid=%d\n",getpid());
    }
    return 0;
}