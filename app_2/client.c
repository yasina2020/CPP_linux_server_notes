/*
客户端-多线程


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
    while(1){
        char *server_ip = inet_ntoa(server_addr.sin_addr);
        int server_port = ntohs(server_addr.sin_port);
        printf("请输入要发送的内容:\n");
        fgets(buf, sizeof(buf), stdin);
        int w_len = write(client_fd,buf,sizeof(buf));
        if(w_len == 0){
            // 读端关闭
            printf("服务器ip:%s,port:%d已关闭,客户端即将退出...\n",server_ip,server_port);
            break;
        }else if(w_len == -1){
            perror("write:");
            break;
        }else if(w_len > 0){
            printf("客户端已发送数据。\n");
        }
        bzero(buf,sizeof(buf));
        int r_len = read(client_fd,buf,sizeof(buf));
        if(r_len == 0){
            // 写端关闭
            printf("服务器ip:%s,port:%d已关闭,客户端即将退出...\n",server_ip,server_port);
            break;
        }else if(r_len == -1){
            perror("write:");
            break;
        }else{
            printf("服务器ip:%s,port:%d发送的数据是:\n",server_ip,server_port);
            printf("%s\n",buf);
        }
        bzero(buf,sizeof(buf));
        
    }
    close(client_fd);
    return 0;
}