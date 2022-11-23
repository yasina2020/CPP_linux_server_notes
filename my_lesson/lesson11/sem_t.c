/*
信号量  信号灯
初始化时会设置有几个灯，当所有的灯都灭了，就阻塞，没办法访问
#include <semaphore.h>

信号量的数据类型 sem_t
int sem_init(sem_t *sem, int pshared, unsigned int value);
初始化信号量
    -sem
    -pshared 代表用在线程还是进程之间，线程:0  非零:进程
    -value 记录信号量的值（即有几个灯）
int sem_destory(sem_t *sem);
销毁信号量对象
int sem_wait(sem_t *sem);
如果值为0，阻塞，否则对信号量的值减一，直到>0。
int sem_trywait(sem_t *sem);

int sem_timewait(sem_t *sem,const struct timespace *sbs_timeout);

int sem_post(sem_t *sem);
调用后对信号量的值加一，解除阻塞
int sem_getvalue(sem_t *sem,int *sval);
获取信号量的值
    -sval 传出参数


案例：生产者消费者
    仓库大小 8
    当仓库为空的时候，消费者线程应该阻塞，然后通知生产者线程进行生产
    每当生产者生产出来一个产品后，就会通知消费者进行消费
    当仓库满时，生产者阻塞，消费者进行消费
*/ 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

struct Node{
    int num;
    struct Node * next;
};
struct Node * head= NULL;


// 先保证共享仓库的数据安全，临界区原子性
pthread_mutex_t mutex;
// 信号量
sem_t psem;
sem_t csem;

void *produce(void * arg){
    while(1){
        sleep(1);
        // 信号量在这里等待，如果有剩余的空位，就继续，否则就阻塞
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        //生产者新增节点 头插
        struct Node* new_node = ( struct Node*)malloc(sizeof(struct Node));
        new_node->next = head;
        head = new_node;
        new_node->num = rand()%100;
        printf("生产者%ld生产了一个节点%d\n",pthread_self(),new_node->num);
        pthread_mutex_unlock(&mutex);
        // 生产出来一个 通知消费者来消费（即可以消费的信号量空位多一个）
        sem_post(&csem);
    }
    return NULL;
}
void *consum(void * arg){
    while(1){
        sleep(1);
        // 信号量在这里等待，如果有可以消费的空位，就继续，否则就阻塞
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node* old_node = head;
        head = head->next;
        printf("消费者%ld删除了一个节点%d\n",pthread_self(),old_node->num);
        free(old_node);
        pthread_mutex_unlock(&mutex);
        // 消费完之后，仓库会多出来一个位置，
        sem_post(&psem);  
    }
    return NULL;
}

int main(){


    // 初始化锁和信号量
    pthread_mutex_init(&mutex,NULL);
    sem_init(&psem,0,8);
    sem_init(&csem,0,0);

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

    // 释放锁和sem对象资源
    pthread_mutex_destroy(&mutex);
    sem_destroy(&psem);
    sem_destroy(&csem);

    pthread_exit(NULL);

    return 0;
}

