/*
客户端-多进程


*/ 

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>

int main(){
// 1、创建业务fd
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
// 2、连接服务器
    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.s_addr);
    connect(client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    
// 3、业务处理
    char buf[1024] = {0};
    int i;
    while(1){
        sleep(2);
        sprintf(buf,"client write data:%d\n",i++);
        int w_len = write(client_fd,buf,sizeof(buf));
        if(w_len == 0){
            printf("server is close...\n");
        }else if(w_len == -1){
            perror("write:");
            exit(0);
        }
        memset(buf,0,sizeof(buf));
        int r_len = read(client_fd,buf,sizeof(buf));
        printf("server send data is : %s\n",buf);
    }
    close(client_fd);
    return 0;
}