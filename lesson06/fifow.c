// 客户端B
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
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
    // 打开写管道fifo1
    int fifo_w = open(fifo1,O_WRONLY);
    //写
    time_t timep;
    msg write_msg;
    while (1) {

        time(&timep);
        struct tm t = *gmtime(&timep);
        write_msg.p = t;
        strcpy(write_msg.sender, "xiaohong");
        strcpy(write_msg.rever, "Jane");
        printf("输入内容：");
        printf("发送时间:%d年%d月%d日\n",t.tm_year+1990,t.tm_mon+1,t.tm_mday);
        scanf("%s", write_msg.message);
        write(fifo_w,&write_msg,sizeof(write_msg));
    }
        
}

