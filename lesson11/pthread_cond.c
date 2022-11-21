/*
条件变量
    达到某一条件 阻塞
    达到某一条件 解除阻塞
条件变量类型 pthread_cond_t
int pthread_cond_init (pthread_cond_t *__restrict __cond,
			      const pthread_condattr_t *__restrict __cond_attr);
    初始化条件变量
int pthread_cond_destroy (pthread_cond_t *__cond);
    销毁条件变量对象
int pthread_cond_signal (pthread_cond_t *__cond);
    解除该变量的阻塞
int pthread_cond_broadcast (pthread_cond_t *__cond);
    解除该变量的阻塞（通知所有线程）
int pthread_cond_wait (pthread_cond_t *__restrict __cond,
			      pthread_mutex_t *__restrict __mutex);
    阻塞该变量：当该函数生效时，会释放mutex锁，等到阻塞状态被解除之后，又会自动加上mutex锁
int pthread_cond_timedwait (pthread_cond_t *__restrict __cond,
				   pthread_mutex_t *__restrict __mutex,
				   const struct timespec *__restrict __abstime);
    阻塞该变量（时间到达后若还在阻塞，就不执行）



案例：生产者消费者（只考虑仓库空，不考虑仓库满的情况）
    当仓库为空的时候，消费者线程应该阻塞，然后通知生产者线程进行生产
    每当生产者生产出来一个产品后，就会通知消费者进行消费

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


// 先保证共享仓库的数据安全，临界区原子性
pthread_mutex_t mutex;
// 条件变量来阻塞消费者和解除阻塞消费者（仅考虑仓库空的情况）
pthread_cond_t cond;

void *produce(void * arg){


    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        //生产者新增节点 头插
        struct Node* new_node = ( struct Node*)malloc(sizeof(struct Node));
        new_node->next = head;
        head = new_node;
        new_node->num = rand()%100;
        // 生产出来一个 通知消费者来消费
        pthread_cond_signal(&cond);
        printf("生产者%ld生产了一个节点%d\n",pthread_self(),new_node->num);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}
void *consum(void * arg){
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        // 消费者删除节点  头删
        if(head == NULL){//仓库空，阻塞
            printf("消费者%ld发现仓库为空,阻塞\n",pthread_self());
            pthread_cond_wait(&cond,&mutex);
        // -----------------------我的一点想法---------------------------
        // 注：pthread_cond_wait在阻塞的同时，会释放锁，
        // 但是一旦将所释放出来，有可能cpu又被其他消费者线程抢了过去
        // 极端的情况下，仓库会一直为空，不同的消费者会一直抢占CPU不给生产者生产
        // 因此最优的解法是：阻塞后，下一个抢占CPU的线程一定是生产者。
        // ---------------------------------------------------------------
            // pthread_mutex_unlock(&mutex);
        }
        //前面的程序会一直阻塞，直到有产品了，就会解除阻塞 ，然后来消费。
        struct Node* old_node = head;
        head = head->next;
        printf("消费者%ld删除了一个节点%d\n",pthread_self(),old_node->num);
        free(old_node);
        pthread_mutex_unlock(&mutex);
        
    }

    return NULL;
}

int main(){


    // 初始化锁和条件变量
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    pthread_t producer[5],consumer[5];

    for(int i=0; i<5; i++){
        pthread_create(&producer[i],NULL,produce,NULL);
        pthread_create(&consumer[i],NULL,consum,NULL);
    }

    for(int i=0; i<5; i++){
        pthread_detach(producer[i]);
        pthread_detach(consumer[i]);
    }

    while(1){
        sleep(1);
    }

    // 释放锁和条件变量对象资源
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    pthread_exit(NULL);

    return 0;
}

