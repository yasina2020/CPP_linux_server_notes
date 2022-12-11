#ifndef LINK_H
#define LINK_H
#include <string>

/*
要实现一个链表
链表头中仅存储该链表的信息
功能：
    头插
    尾插
    头删
    尾删
    位置插入
    遍历
    清空
*/
namespace m_link{


class Node{
public:
    Node(){
        next = nullptr;
    }

public:
    void* data;
    Node* next;
};

class Link{
public:
    // // 构造
    Link();
    // 返回长度
    int getLength();
    // 头插
    Link* insterHead(void *data);
    // 尾插
    Link* insterRear(void *data);
    // 头删
    Link *deleteHead();
    // 尾删 XX

    // 位置插入从0开始
    Link* insterIndex(void *data,int index);
    // 遍历 因为数据类型不去确定，所以设计一个回调函数，让用户可以自定义打印行为。
    void printLink(void(*printFunc)(void* arg));
    // 清空 
    void clareLink();

private:
    int size;//Link length
    Node *head;
    Node *rear;
};

}

#endif