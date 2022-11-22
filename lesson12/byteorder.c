/*
字节序
小端模式：低地址存低位，高地址存高位
大端模式：低地址存高位，高地址存低位

*/ 
#include<stdio.h>
// 判断字节序
void byteorder(){
    int a = 0x0102;
    printf("%d\n",*(char *)&a);
    /*
    这里int是两个字节
    char是一个字节
    int会开辟一个两个字节大小的内存，
    然后用char指针指向这块内存，自然指向的就是第一个字节
    也就是说char的指针只能识别到一个字节大小的内存。
    故解析出来的值就是低内存存储的数。
    
    */ 
    int x = *(char *)&a;
    if(x == 2){
        printf("小端模式：低地址存低位，高地址存高位\n");
    }else if(x == 1){
        printf("大端模式：低地址存高位，高地址存低位\n");
    }
    return 0;
}



/*
字节序转换函数
网络字节序：大端模式

#include <arpa/inet.h>

// 一般用来转端口
uint16_t htons(uint16_t hostshort); //主机到网络
uint16_t ntohs(uint16_t netshort); //网络到主机
// 一般用来转IP
uint32_t hton1(uint32_t hostlong);
uint32_t ntohl(uint32_t netlong);

含义n:net  h:host  s:short 2字节  l:long 4字节
*/ 


int mian(){
    byteorder();
    return 0;
}