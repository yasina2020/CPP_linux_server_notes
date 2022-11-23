/*
    守护进程，每隔两秒获取一下系统时间，写入文件
*/ 

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE


void my_call(){
    int fd2w = open("wfile.txt",O_CREAT|O_RDWR|O_APPEND,0664);
    if(fd2w == -1){
        perror("open");
        exit(0);
    }

    time_t timep = time(NULL);
    struct tm *t = localtime(&timep);
    // char buf[1024];
    // sprintf(buf,"%d年%d月%d日%d时%d分%d秒\n",t->tm_year+1900,
    // t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

    char * stime = asctime(t);
    int len = write(fd2w,stime,strlen(stime));
    if(len == -1){
        perror("write");
        exit(0);
    }
    // printf("%s\n",stime);
    close(fd2w);
}


int main(){
    // 创建子进程
    pid_t pid = fork();
    // 退出父进程 
    if(pid > 0){
        exit(0);
    }else if (pid == -1){
        perror("fork");
        exit(0);
    }
    //在子进程中创建会话 
    setsid();
    // 更改子进程权限和工作目录
    umask(022);
    chdir("/home/fang/newcodercpp/lesson10/");
    // 关闭和重定向文件描述符
    int fd = open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);
    // 处理业务 
        //注册信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = my_call;
        sigemptyset(&act.sa_mask);
        if(sigaction(SIGALRM,&act,NULL)==-1){
            perror("sigaction");
            exit(0);
        }
        
        // 设置2秒的定时器
        struct itimerval it;
        it.it_interval.tv_sec = 2;
        it.it_interval.tv_usec = 0;
        it.it_value.tv_sec = 2;
        it.it_value.tv_usec = 0;
        if(setitimer(ITIMER_REAL,&it,NULL)==-1){
            perror("setitimer");
            exit(0);
        };

        // 写文件
        while(1){
            sleep(5);
        }
    return 0;
}