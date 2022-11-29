/* UDP*/ 

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

    int num = 0;
    while(1){
        char buf[128];
        int addr_len = sizeof(addr);
        char server_ip[16]; 

        // 发送数据
        sprintf(buf,"I am num:%d\n",num++);
        sendto(fd,buf,strlen(buf)+1,0,(struct sockaddr*)&addr,addr_len);

        // 接收数据
        recvfrom(fd,buf,sizeof(buf),0,(struct sockaddr*)&addr,&addr_len);
        printf("服务器信息IP:%s,PORT:%d\n",
                inet_ntop(AF_INET,(void *)&addr.sin_addr.s_addr,server_ip,sizeof(server_ip)),
                ntohs(addr.sin_port));
        printf("%s",buf);
        
        printf("---------------------\n");
        sleep(1);
    }
    close(fd);
    return 0;
}