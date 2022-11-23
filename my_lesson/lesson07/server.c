// 服务端


/*  
当客户端A与B发起通信时
服务端创建创建两个管道文件

这里简单实现，假设客户端运行前，服务端已经将管道创建好了

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
    printf("检测到连接\n");
    char *fifo1 = "/home/fang/newcodercpp/lesson07/fifo1";
    char *fifo2 = "/home/fang/newcodercpp/lesson07/fifo2";

    if(access(fifo1,F_OK)==-1){
        printf("为检测到管道文件，创建管道文件\n");
        if(mkfifo(fifo1,0664)==-1){
            perror("fifo1");
            exit(0);
        }
        if(mkfifo(fifo2,0664)==-1){
            perror("fifo2");
            exit(0);
        }
        printf("创建管道文件完成\n");
    }
    return 0;
}