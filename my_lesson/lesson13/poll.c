#include <poll.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CLIENT 1024

struct pollfd fds[MAX_CLIENT];

// 业务处理
void yewu_chuli(int i){
    int client_fd = fds[i].fd;
    char rev_buf[1024];
    int len = read(client_fd,rev_buf,sizeof(rev_buf));
    if(len == -1){
        // -1读数据失败
        perror("read:");
        exit(0);
    }else if(len == 0){
        // 0 没读到数据（写端关闭）
        // 当客户端退出时，再将fd置为-1；
        printf("客户端不发送数据，连接断开\n");
        fds[i].fd = -1;
        close(client_fd);
    }else if(len > 0){
        // 读到数据打印
        printf("他发送的数据是:%s\n",rev_buf);
        // 给客户端发送数据
        sprintf(rev_buf,"服务器已接受到数据.\n");
        int w_len = write(client_fd,rev_buf,sizeof(rev_buf));
        if(w_len>0){
            printf("服务器写入成功\n");
        }else if(w_len ==-1){
            perror("write");
            exit(0);
        }
        
    }
    
}



int main(){
    int listen_fd = socket(PF_INET,SOCK_STREAM,0);

    printf("监听fd%d\n",listen_fd);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));

    listen(listen_fd,128);


    for(int i=0;i<MAX_CLIENT;i++){
        fds[i].fd=-1;
        fds[i].events=POLLIN;
        fds[i].revents = 0;
    }
    fds[0].fd = listen_fd;
    int nfds = 0;

    while(1){
        int ret = poll(fds,nfds+1,-1);
        if(ret == -1){
            perror("poll");
            exit(0);
        }else if(ret > 0){
            // 有IO操作
            // 1、表明有新的客户端接入
            if(fds[0].revents & POLLIN){
                struct sockaddr_in client_addr;
                int client_addr_len = sizeof(client_addr);
                int client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_addr_len);
                // 将新的客户端加入到poll的监听队列中
                for(int i=1;i<MAX_CLIENT;i++){
                    if(fds[i].fd == -1){
                        fds[i].fd=client_fd;
                        fds[i].events = POLLIN;
                        fds[i].revents = 0;
                        printf("i=%d,添加客户端%d\n",i,fds[i].fd);
                        break;
                    }
                    // 如果没有空闲的，就....俺也不知道
                }
                nfds++;
            }

            // 2、循环遍历产生I/O操作得client_fd,然后对其进行业务处理
            for(int i = 1;i <= nfds;i++){
                if((fds[i].fd != -1) && (fds[i].revents & POLLIN) ){
                    printf("i=%d,进入%d的业务\n",i,fds[i].fd);
                    yewu_chuli(i);
                    // nfds--;
                    printf("i=%d,%d的业务完毕\n",i,fds[i].fd);
                    // fds[i].fd=-1;
                }
            }
        }
    }


    close(listen_fd);
    return 0;
}


