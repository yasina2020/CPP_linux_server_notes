/* UDP
#include <sys/types.h>
#include <sys/socket.h>
ssize_t sendto(int sockfd,const void *buf,size_t len,int flags,
                const struct sockaddr *dest_addr,socklen_t addr_len);
把buf通过fd发送给dest_addr
-flags 设置标志 一般用0

ssize_t recvfrom(int sockfd,void *buf,size_t len,int flags,
                struct sockaddr *src_addr,socklen_t *addr_len);
从sockfd接收buf
-src_addr发送数据方的信息
返回值 -1失败  >0 接收的数据大小

*/ 
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(){

    int fd = socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    bind(fd,(struct sockaddr*)&addr,sizeof(addr));

    while(1){
        char buf[128];
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        char client_ip[16]; 

        // 接收数据
        recvfrom(fd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&client_addr_len);
        printf("客户端信息IP:%s,PORT:%d\n",
                inet_ntop(AF_INET,(void *)&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
                ntohs(client_addr.sin_port));
        printf("%s",buf);
        // 发送数据
        sendto(fd,buf,strlen(buf)+1,0,(struct sockaddr*)&client_addr,client_addr_len);
        printf("---------------------\n");
    }
    close(fd);
    return 0;
}