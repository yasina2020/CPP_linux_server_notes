#include <sys/epoll.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLINET 1024

// 业务处理
void yewu_chuli(int client_fd){
    
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

    int epoll_fd = epoll_create(1);
    struct epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN;
    // 添加到红黑树里
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&event);

    // 设置一个用于存储产生IO操作的文件描述符的数组
    struct epoll_event events[MAX_CLINET];

    while (1){
        int ret = epoll_wait(epoll_fd,events,MAX_CLINET,-1);
        printf("ret =%d\n",ret);
        if(ret == -1){
            perror("epoll_wait");
            exit(0);
        }
        for(int i=0;i<ret;i++){
            if(events[i].data.fd == listen_fd){
            // 如果是新客户端加入
                struct sockaddr_in clinet_addr;
                int clinet_addr_len = sizeof(clinet_addr);
                int client_fd = accept(listen_fd,(struct sockaddr*)&clinet_addr,&clinet_addr_len);
                
                event.data.fd = client_fd;
                event.events = EPOLLIN;
                epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&event);
                
                printf("添加客户端%d\n",client_fd);
            }else{
            // 业务处理---------------------------------------------
                printf("进入%d的业务\n",events[i].data.fd);
                char rev_buf[1024];
                int len = read(events[i].data.fd,rev_buf,sizeof(rev_buf));
                if(len == -1){
                    // -1读数据失败
                    perror("read:");
                    exit(0);
                }else if(len == 0){
                    // 0 没读到数据（写端关闭）
                    // 当客户端退出时，再将fd移除；
                    printf("客户端不发送数据，连接断开\n");
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                }else if(len > 0){
                    // 读到数据打印
                    printf("他发送的数据是:%s\n",rev_buf);
                    // 给客户端发送数据
                    sprintf(rev_buf,"服务器已接受到数据.\n");
                    int w_len = write(events[i].data.fd,rev_buf,sizeof(rev_buf));
                    if(w_len>0){
                        printf("服务器写入成功\n");
                    }else if(w_len ==-1){
                        perror("write");
                        exit(0);
                    } 
                }
                printf("%d的业务完毕\n",events[i].data.fd);    
            // 业务处理---------------------------------------------
            }
            
        }
    }
    
    close(listen_fd);
    close(epoll_fd);

    return 0;
}