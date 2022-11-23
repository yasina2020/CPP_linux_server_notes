/*
socket 套接字

通用socket地址
#include <bits/socket.h>
struct sockaddr{
    sa_family_t sa_family;
    cahr sa_data[14];
};


专用socket地址（比通用的方便）
struct sockaddr_in{
    sa_family_t sa_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    ...
};
    sa_family:
        PF_UNIX       AF_UNIX     UNIX
        PF_INET       AF_INET     TCP/IPv4
        PF_INET6      AF_INET6    TCP/IPv6
    in_addr:
        struct in_addr{
            in_addr_t s_addr;
        }
注：所有专用的socket地址再实际使用时都需要转换成通用socketaddr类型
(socketaddr)socketaddr_in


IP地址转换函数
-将字符串类型的（十进制/十六进制) IP 转换为 整数类型的二进制，
#include <arpa/inet.h>


in_addr_t inet_addr(const char *cp);
// 字符转为网络字节序的整数

int inet_aton(const char *cp,struct in_addr *inp);
// 字符转化为网络字节序的整数并保存在inp中
返回值 1成功 0失败

char *inrt_ntoa(struct in_addr in);
// 网络字节序的IP转化为字符串类型

// 新的API
int inet_pton(int af,const char *src,void *dst);
          inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.s_addr);
//点分十进制的字符串 转化 为网络字节序的整数

const char *inet_ntop(int af,const void*src, char *dst,socklen_t size);
//网络字节序的整数 转化 为点分十进制的字符串
返回转换后的数据的地址 和dst一样

af:AF_INET、AF_INET6
dst:传出参数
size:指定dst的长度
*/ 
/*
套接字函数
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>  //该头文件包含上面两个头文件

int socket(int domain, int type, int protocol);
创建一个套接字fd
-domain：
    AF_UNIX 本机
    AF_INET ipv4
    AF_INET6 ipv6
-type:
    SOCK_STREAM 流式协议 一般指TCP
    SOCK_DGRAM 报式协议 一般指UDP
-protocol:
    一般是0
返回值：成功返回最小的可用fd，失败返回-1

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
套接字fd和ip，port绑定
-sockfd
-addr 通用socket地址的指针，使用：(sockaddr*) socketaddr_in
-addrlen 内存大小（sizeof）

int listen(int sockfd, int backlog);// /proc/sys/net/core/somaxconn
开启监听
-sockfd
-backlog:未连接队列+已连接队列 的最大值
返回值：0成功  -1失败
注：accept会从已连接队列中取出来一个套接字使用

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
接收客户端连接（阻塞，等待从已连接队列中取连接fd）
-sockfd
-addr：传出参数，记录客户端的信息
-addrlen（指针，取地址）
返回值：成功返回用于通信的fd，失败-1

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
客户端连接服务器
-sockfd 用于通信的文件描述符
-addr 要连接的服务器的信息
-addrlen
返回值：成功0，失败-1

#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
写数据
ssize t read(int fd, void *buf, size t count);
读数据

*/ 