/*
生产者消费者

案例：创建五个生产者，5个消费者，一个链表，消费者删节点，生产者头插节点
*/ 





#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct Node{
    int num;
    struct Node * next;
};
struct Node * head= NULL;


void *produce(void * arg){


    while(1){
        //生产者新增节点 头插
        struct Node* new_node = ( struct Node*)malloc(sizeof(struct Node));
        new_node->next = head;
        head = new_node;
        new_node->num = rand()%100;
        printf("生产者%ld生产了一个节点%d\n",pthread_self(),new_node->num);
    }
    return NULL;
}
void *consum(void * arg){
    while(1){
        // 消费者删除节点  头删
        struct Node* old_node = head;
        head = head->next;
        printf("消费者%ld删除了一个节点%d\n",pthread_self(),old_node->num);
        free(old_node);
    }

    return NULL;
}

int main(){
    pthread_t producer[5],consumer[5];

    for(int i=0; i<5; i++){
        pthread_create(&producer[i],NULL,produce,NULL);
        pthread_create(&consumer[i],NULL,consum,NULL);
    }

    for(int i=0; i<5; i++){
        pthread_detach(producer[i]);
        pthread_detach(consumer[i]);
    }

    pthread_exit(NULL);

    return 0;
}

