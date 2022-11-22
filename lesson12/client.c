/*
TCP通信的客户端

*/ 

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>

int main(){
// 1、创建业务fd
    int yewu_fd = socket(AF_INET,SOCK_STREAM,0);
// 2、连接服务器
    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(2202);
    inet_pton(AF_INET,"127.0.0.1",server_addr.sin_addr.s_addr);
    connect(yewu_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    
// 3、业务处理
    char buf[1024];
    read(yewu_fd,buf,sizeof(buf));
    printf("%s\n",buf);
    return 0;
}