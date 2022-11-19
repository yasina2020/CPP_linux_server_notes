// 客户端A
/*
A与B通信，A和B都将随意的读写。
A创建一个子进程，父进程读，子进程写
B同理

AB通信的消息结构体

struct MSG{
    struct tm *p;
    char message[1024];
    char sender[10];
    char rever[10];
    ...
};


*/ 
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef struct MSG{
    struct tm p;
    char message[1024];
    char sender[10];
    char rever[10];
} msg;


char *fifo1 = "/home/fang/newcodercpp/lesson07/fifo1";
char *fifo2 = "/home/fang/newcodercpp/lesson07/fifo2";

int main(){
    // 打开读管道fifo1
    int fifo_r = open(fifo1,O_RDONLY);
    //读
    

    while(1){
        // sleep(1);
        // 读整个结构体
        msg read_msg;
        read(fifo_r,&read_msg,sizeof(read_msg));
        printf("发送者:%s 接收者:%s\n",read_msg.sender,read_msg.rever);
        printf("发送时间:%d年%d月%d日\n",read_msg.p.tm_year+1990,read_msg.p.tm_mon+1,read_msg.p.tm_mday);
        // printf("发送时间:%d年%d月%d日%d时%d分%d秒\n",p->tm_year+1990,p->tm_mon+1,p->tm_mday,p->tm_hour+8,p->tm_min,p->tm_sec);
        printf("消息内容:%s\n",read_msg.message);
    }
   
}