/*
匿名管道只能用在有亲缘关系的进程之间通信，fork后子进程共享父进程的文件描述符表，故父子进程中都有读写端。

有名管道：
        以文件的形式存在于系统中，但是文件仅供两个进程之间通信时能够找到同一个文件描述符，
    并不会向这个文件实体写数据，仍是往内核缓冲区（内存）中写数据。
        因此通过这个文件，不相关的进程也能通信。
        即使使用fifo的进程退出，fifo文件也不会消失，和读写普通文件一样。

创建：
    命令行
        mkfifo fifoname
    函数
        #include <sys/types.h>
        #include <sys/stat.h>

        int mkfifo(const char  *pathname,mode_t mode);
        -mode:文件权限和open的参数一样
        返回值 成功0 失败-1并设置errno

*/


#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    char *pathname = "/home/fang/newcodercpp/lesson06/my_fifo";
    // 判断是否存在
    if(access(pathname,F_OK)==-1){
        if(mkfifo(pathname,0664) == -1){
            perror("fifo");
            exit(0);
        }
    }
    
    return 0;
}