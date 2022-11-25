#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>

// 业务处理
void yewu_chuli(int client_fd,fd_set *set){

    char rev_buf[1024];
    int len = read(client_fd,rev_buf,sizeof(rev_buf));
    if(len == -1){
        // -1读数据失败
        perror("read:");
        exit(0);
    }else if(len == 0){
        // 0 没读到数据（写端关闭）
        printf("客户端不发送数据，连接断开\n");
        close(client_fd);
        FD_CLR(client_fd,set);
    }else if(len > 0){
        // 读到数据打印
        printf("他发送的数据是:%s\n",rev_buf);
    }
    // 给客户端发送数据
    sprintf(rev_buf,"服务器已接受到数据.\n");
    write(client_fd,rev_buf,sizeof(rev_buf));
}


int main(){

    int listen_fd = socket(PF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));

    listen(listen_fd,128);

    fd_set tmp,rset;
    FD_ZERO(&rset);
    FD_SET(listen_fd,&rset);
    int max_fd = listen_fd;

    while(1){
        tmp = rset;
        int ret = select(max_fd + 1,&tmp,NULL,NULL,NULL);
        if(ret == -1){
            perror("select");
            exit(0);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 说明tmp中有I/O操作
            if(FD_ISSET(listen_fd,&tmp)){
                // 如果是listen_fd的IO操作，说明有新的客户端连接进来了
                struct sockaddr_in client_addr;
                int client_addr_len = sizeof(client_addr);
                int client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_addr_len);
                if(client_fd == -1){
                    perror("accept");
                    exit(0);
                }else{
                    FD_SET(client_fd,&rset);
                    FD_SET(client_fd,&tmp);
                    max_fd = max_fd>client_fd?max_fd:client_fd;
                }
            }
            // 循环遍历产生I/O操作得client_fd,然后对其进行业务处理
            for(int i=listen_fd + 1;i<=max_fd;i++){
                if(FD_ISSET(i,&tmp)){
                    yewu_chuli(i,&rset);
                }
            }
        }
    
    }
    close(listen_fd);
    
    return 0;
}