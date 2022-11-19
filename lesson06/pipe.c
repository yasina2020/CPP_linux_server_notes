/*
#include <unistd.h>

创建匿名管道
int pipe(int pipefd[2]);
    匿名管道只能用于有关系的进程通信（父子，兄弟，祖孙）
    传出参数：
        pipefd[0] 读端
        pipefd[1] 写端
    返回值：
        0 成功  -1 失败

管道（循环队列）是默认阻塞的，如果管道为空，read阻塞，如果管道满了 wirte阻塞；



查看管道缓冲大小命令
ulimit -a

查看管道缓冲大小函数
long fpathconf(int fd, int name);

long size = fpathconf(pipefd[0],_PC_PIPE_BUF);

*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
    //在创建子进程之前创建管道。
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    
    if(pid==0){
        // 子进程  从管道写数据 写端 pipefd[1] 
        char buf[1024] = {0};
        while (1)
        {
            char *str = "22222222222222222222";
            write(pipefd[1],str,strlen(str));
            printf("write over\n");
        }
    }else if(pid > 0){
        //父进程 从管道读端  读数据 pipefd[0]
        char buf[1024] = {0};
        while(1){
            
            int len = read(pipefd[0],buf,sizeof(buf));
            printf("parent pid %d,reve size:%d,date:%s\n",getpid(),len,buf);          
        }

    }

}

