/*
服务器端--多进程
思路：
1、父进程循环阻塞在accept，
2、当接收到accept后，解除阻塞，且创建子进程
3、在子进程中处理业务
4、关闭业务fd，退出子进程，
5、父进程关闭监听fd

*/ 

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/sigaction.h>

void my_call(int arg){
    while(1){
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == 0){
            // 还有未结束的子进程
            break;
        }else if(ret == -1){
            // 所有的子进程都已经回收了
            break;
        }else{
            printf("child precosse was done that pid is %d\n",ret);
        }
    }
}




int main(){

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = my_call;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD,&act,NULL);

    int l_fd = socket(PF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3333);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.s_addr);
    
    bind(l_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));

    listen(l_fd,128);

    while(1){
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        int c_fd = accept(l_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(c_fd == -1){
            if(errno == EINTR){
                continue;
            }
            perror("accept c_fd");
            exit(0);
        }
        pid_t pid = fork();
        if(pid == 0){
        // 在子进程里处理业务(循环)
            while(1){
                // 1、显示客户端信息
                char * client_ip = inet_ntoa(client_addr.sin_addr);
                int client_port = ntohs(client_addr.sin_port);
                printf("client ip:%s,port:%d\n",client_ip,client_port);
                // 2、读客户端发来的数据
                char rev_buf[1024];
                int len = read(c_fd,rev_buf,sizeof(rev_buf));
                if(len == -1){
                    perror("read:");
                    exit(0);
                }else if(len == 0){
                    printf("客户端不发送数据，连接断开");
                }else{
                    printf("client send data is :%s\n",rev_buf);
                }
                // 3、给客户端发送数据
                sprintf(rev_buf,"hi i am server.\n");
                write(c_fd,rev_buf,sizeof(rev_buf));
            }
            // 4、断开客户端fd
            close(c_fd);
            // 5、退出子线程，会发送SIGCHLD信号，故设置捕获函数
            exit(0);
        }
    }

    close(l_fd);

    return 0;
}

