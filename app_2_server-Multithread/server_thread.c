#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <errno.h>

/*
1、socket流程
2、循环接收accept
3、开启新的线程处理业务
4、在业务处理线程中开启两个线程用来读写同时进行
5、业务线程结束后回收资源

注：设置一定数量的可用fd
    即服务器最大只能连接100个客户端
*/ 

#define MAX_CLIENT 2


//C语言在结构体中是不可以直接初始化成员的
struct socket_info{
    int client_fd;//客户端的业务fd
    struct sockaddr_in client_addr;//客户端的连接信息
    pthread_t tid;//为该客户端开辟的线程号
};

struct socket_info client_info[MAX_CLIENT]; 

int have_fd = MAX_CLIENT;
// 客户端业务处理函数
void *my_call(void * arg){
    struct socket_info *client_info =(struct socket_info *)arg;
    char *client_ip = inet_ntoa(client_info->client_addr.sin_addr);
    int client_port = ntohs(client_info->client_addr.sin_port);
    int client_fd = client_info->client_fd;
    int client_tid = client_info->tid;

    char buf[1024]={0};
    while(1){

        
        // 读客户端的数据
        int r_len = read(client_fd,buf,sizeof(buf));
        if(r_len == -1){
            // -1读数据失败
            perror("read:");
            break;
        }else if(r_len == 0){
            // 0 没读到数据（写端关闭）
            printf("客户端ip:%s,port:%d\n",client_ip,client_port);
            printf("客户端不发送数据，连接断开\n");
            break;
        }else if(r_len > 0){
            // 读到数据打印
            printf("客户端ip:%s,port:%d发送的数据是:\n",client_ip,client_port);
            printf("%s\n",buf);
            
        }
        bzero(buf,sizeof(buf));
        // 向客户端写数据

        sprintf(buf,"服务器已接受到客户端ip:%s,port:%d发送的数据.\n",client_ip,client_port);
        int w_len = write(client_fd,buf,sizeof(buf));
        if(w_len == -1){
            // -1写数据失败
            perror("write:");
            break;
        }else if(w_len == 0){
            // 0 没写到数据（读端关闭）
            printf("客户端ip:%s,port:%d\n",client_ip,client_port);
            printf("客户端已连接断开\n");
            break;
        }else if(w_len > 0){
            // 写数据成功
            
        }
        bzero(buf,sizeof(buf));
    }

    client_info->client_fd = -1;
    client_info->tid = -1;
    have_fd++;
    return NULL;
}

int main(){
// socket流程
    int listen_fd = socket(PF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));

    listen(listen_fd,128);

    
    for(int i=0;i < MAX_CLIENT;i++){
        bzero(&client_info[i],sizeof(client_info[0]));
        client_info[i].client_fd = -1;
        client_info[i].tid = -1;
    }

    while(1){
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        
        while(have_fd == 0){
            sleep(1);
        }
        // 接收新客户端连接
        int client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_addr_len);
        if(client_fd == -1){
            if(errno == EINTR){
                continue;
            }
            perror("accept client_fd");
            exit(0);
        }

        for(int i=0;i < MAX_CLIENT;i++){
            printf("i:%d,fd:%d,tid:%ld\n",i,client_info[i].client_fd,client_info[i].tid);
            // 找到一个没有被使用的i
            if(i == MAX_CLIENT - 1 && client_info[i].client_fd != -1){
                i = 0;
                break;
            }
            // 找到之后，创建一个线程来处理业务
            if(client_info[i].client_fd == -1){
                client_info[i].client_fd = client_fd;
                memcpy(&client_info[i].client_addr,&client_addr,sizeof(client_addr));

                pthread_create(&client_info[i].tid,NULL,my_call,&client_info[i]);

                pthread_detach(client_info[i].tid);

                have_fd--;
                break;
            }
        }
        
    }
    return 0;
}