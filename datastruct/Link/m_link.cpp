// 链表
#include "m_link.hpp"
#include <iostream>

using namespace m_link;

// 构造
Link::Link(){
    this->size = 0;
    this->head = nullptr;
    this->rear = nullptr; 
}

// 返回长度
int Link::getLength(){
    return this->size;
}
// 头插
Link* Link::insterHead(void *data){
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    if(this->head == nullptr){
        this->size ++;
        this->head=node;
        this->rear=node;
    }else{
        node->next = this->head;
        this->head = node;
        this->size ++;
    }
    return this;
}
// 尾插
Link* Link::insterRear(void *data){
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    if(this->head == nullptr){
        this->size ++;
        this->head=node;
        this->rear=node;
    }else{
        this->rear->next = node;
        this->rear = node;
        this->size ++;
    }
    return this;
}
// 头删
Link* Link::deleteHead(){
    if(this->head=nullptr){
        return nullptr;
    }else{
        Node *delnode = this->head;
        this->head = this->head->next;
        this->size --;
        delete delnode;
        return this;
    }
}
// 尾删 XX

// 位置插入
Link* Link::insterIndex(void *data,int index){
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    if(this->size < index){
        return nullptr;
    }else if(index == this->size){
        return insterRear(data);
    }else{
        // 找到要插入的位置的前一个位置
        Node* pcur = this->head;
        for(int i=0;i<index-1;i++){
            pcur ++;
        }
        node->next = pcur->next;
        pcur->next = node;
        this->size ++;
        return this;
    }
}
// 遍历 因为数据类型不去确定，所以设计一个回调函数，让用户可以自定义打印行为。
void Link::printLink(void(*printFunc)(void* arg)){
    Node *pcur = this->head;
    while(pcur!=this->rear->next){
        printFunc(pcur->data);
        pcur = pcur->next;
    }
}
// 清空 
void Link::clareLink(){
    Node* pcur = this->head;
    while(pcur!=nullptr){
        this->head = this->head->next;
        delete pcur;
        this->size--;
        pcur = this->head;
    }

}