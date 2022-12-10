// 链表
#include <stdio.h>
#include <stdlib.h>
// 1、结构设计
typedef struct Node{
    // void *data;
    int data;
    struct Node *next;
}Node;
// 链表头设计为一个指针，一个大小
typedef struct Link{
    int size;
    Node *head;
}Link;
// 2、功能设计
// 初始化
Link *initLink(){
    Link *list = (Link *)malloc(sizeof(Link));
    list->size=0;
    list->head = NULL;
    return list; 
}
Node *initNode(int data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node; 
}
// 增加（头插）
Link *addNodebyHead(Link *list,Node *node){
    if(list->head == NULL){
        list->head = node;
    }else{
        Node *oNode = list->head;
        list->head = node;
        node->next = oNode;
    }
    list->size++;
    return list;
}

// 删除（头删）
Link *deleteNodebyHead(Link *list){
    if(list->head == NULL){
        return NULL;
    }
    Node *node = list->head;
    list->head = node->next;
    list->size--;
    return list;
}
// 插入
Link *insterNode(Link *list,Node *node_t,int index){
    if(list->size < index){
        printf("inster NULL\n");
        return list;
    }
    if(index == 0){
        return addNodebyHead(list,node_t);
    }
    Node *node = list->head;
    Node *node_l = NULL;
    for(int i=0;i<index;i++){
        node_l = node;
        node = node->next;
    }

    node_t->next = node;
    node_l->next = node_t;

    list->size++;
    return list;
}

// 3、功能测试
int PLink(Link *list){
    if(list->size==0||list == NULL){
        printf("NULL\n");
        return 0;
    }
    Node *pcur = list->head;
    for(int i=0;i<list->size;i++){
        printf("%d--",pcur->data);
        pcur = pcur->next;
    }

    printf("\nsize:%d\n",list->size);
    
    return 0;
}
int main(){
    Link *l = initLink();
    Node *node = initNode(10);
    l = addNodebyHead(l,node);
    PLink(l);
    node = initNode(11);
    l = addNodebyHead(l,node);
    node = initNode(12);
    l = addNodebyHead(l,node);
    node = initNode(13);
    l = addNodebyHead(l,node);
    PLink(l);
    l = deleteNodebyHead(l);
    PLink(l);
    node = initNode(20);
    l = insterNode(l,node,3);
    PLink(l);
}