/*
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *wstatus, int options);
功能：回收指定PID的进程，并且可以设置是否阻塞（options）
参数：
    pid：>0 回收id = pid的子进程，
        =0 回收当前进程组(PGID)的所有子进程，
        =-1 回收所有的子进程，相当于wait(),
        <-1 回收指定进程组（PGID=-pid）中的的子进程（绝对值）
    options：设置阻塞或非阻塞 0 阻塞   WNOHANG 非阻塞

返回值：
    >0 返回子进程的pid
    =0 非阻塞状态时 表示还有子进程活着
    =-1 错误 或者 没有子进程
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
            int st;
            pid_t ret = wait(&st);
            
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

