/*
TCP通信的服务端

*/ 

#include <stdlib.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>

int main(){

// 1、创建监听fd
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);

// 2、将监听fd和服务器ip，port绑定
    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(2202);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_pton(AF_INET,"127.0.0.1",server_addr.sin_addr.s_addr);

    bind(listen_fd,(const struct sockaddr *)&server_addr,sizeof(server_addr));

// 3、开启监听
    listen(listen_fd,10);
    
// 4、（阻塞）接收连接
    struct sockaddr_in clint_addr;
    socklen_t addrlen = sizeof(clint_addr); 
    int yewu_fd = accept(listen_fd,(struct sockaddr*)&clint_addr,&addrlen);
// 5、业务处理
    char * buf = "我是服务器.html";
    write(yewu_fd,buf,strlen(buf));

    return 0;
}