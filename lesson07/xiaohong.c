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
    if(fifo_w ==-1){
        perror("open fifo2");
        exit(0);
    }
    // 打开读管道fifo2
    int fifo_r = open(fifo2,O_RDONLY);
    if(fifo_r==-1){
        perror("open fifo1");
        exit(0);
    }


    // 创建子进程
    pid_t pid = fork();
    // 子进程中对fifo2读
    if(pid==0){
       msg read_msg;
        while(1){
            // 读整个结构体
            sleep(1);
            int len = read(fifo_r,&read_msg,sizeof(read_msg));
            if(len > 0){
                printf("收到消息：----------------------------------\n");
                printf("发送者:%s 接收者:%s\n",read_msg.sender,read_msg.rever);
                printf("发送时间:%d年%d月%d日\n",read_msg.p.tm_year+1900,read_msg.p.tm_mon+1,read_msg.p.tm_mday);
                printf("消息内容:%s\n",read_msg.message);
                printf("--------------------------------------------\n");
            }else if(len == 0){
                printf("对方已关闭聊天\n");
                exit(0);
            }else{
                perror("read");
                exit(0);
            }
        }
        close(fifo_r);
    }
    // 父进程中对fifo1写
    else if (pid > 0){
        msg write_msg;
        time_t timep;
        while (1) {
            time(&timep);
            struct tm t = *gmtime(&timep);
            write_msg.p = t;
            strcpy(write_msg.sender, "xiaohong");
            strcpy(write_msg.rever, "Jane");
            printf("发送消息：----------------------------------\n");
            printf("请输入要发送的内容:\n");
            fgets(write_msg.message, sizeof(write_msg.message), stdin);
            write(fifo_w,&write_msg,sizeof(write_msg));
            printf("--------------------------------------------\n");
        }
        close(fifo_w);
    }else if(pid==-1){
        perror("fork");
        exit(0);
    }
    return 0;
}
