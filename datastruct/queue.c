// 队列FIFO

#include <stdio.h>
#include <stdlib.h>
// 1、结构设计
typedef struct Node{
    // void *data;
    int data;
    struct Node *next;
}Node;
// 栈头设计为一个指针，一个大小
typedef struct Queue{
    int size;
    Node *header;
    Node *rear;
}Queue;
// 2、功能设计
// 初始化
Queue *initQueue(){
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->size=0;
    queue->header = NULL;
    queue->rear = NULL;
    return queue; 
}
Node *initNode(int data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node; 
}
// 入队
Queue *pushQueue(Queue *queue,Node *node){
    if(queue->header == NULL){
        queue->header = node;
        queue->rear = node;
    }else{
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return queue;
}

// 出队
Node *popQueue(Queue *queue){
    if(queue->header == NULL){
        return NULL;
    }
    Node *node = queue->header;
    queue->header = node->next;
    queue->size--;
    return node;
}

// 3、功能测试
int PQueue(Queue *queue){
    if(queue->size==0||queue == NULL){
        printf("NULL\n");
        return 0;
    }
    Node *pcur = queue->header;
    while(pcur){
        printf("%d--",pcur->data);
        pcur = pcur->next;
    }

    printf("\nsize:%d\n",queue->size);
    
    return 0;
}
int main(){
    Queue *s = initQueue();
    Node *node = initNode(10);
    s = pushQueue(s,node);
    PQueue(s);
    node = initNode(11);
    s = pushQueue(s,node);
    PQueue(s);
    node = initNode(12);
    s = pushQueue(s,node);
    PQueue(s);
    node = initNode(13);
    s = pushQueue(s,node);
    PQueue(s);
    Node *topNode = popQueue(s);
    printf("Queue header is %d\n",topNode->data);
    PQueue(s);

}