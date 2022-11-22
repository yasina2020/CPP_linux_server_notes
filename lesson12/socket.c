/*
socket 套接字

通用socket地址
#include <bits/socket.h>
struct socketaddr{
    sa_family_t sa_family;
    cahr sa_data[14];
};


专用socket地址（比通用的方便）
struct socketaddr_in{
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
//点分十进制的字符串 转化 为网络字节序的整数

const char *inet_ntop(int af,const void*src, char *dst,socklen_t size);
//网络字节序的整数 转化 为点分十进制的字符串
返回转换后的数据的地址 和dst一样

af:AF_INET、AF_INET6
dst:传出参数
size:指定dst的长度
*/ 