/*
线程同步问题
两个线程访问修改临界资源时，一定要保证临界区操作的原子性。

解决方法：
1、互斥量 mutex
    mutex有加锁和未枷锁两种状态，最多只有一个线程处于该mutex的加锁状态，
    其他试图加锁的线程会阻塞或者报错

函数方法：
    互斥量类型 pthread_mutex_t
    int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                const pthread_mutexattr_t *restrict attr);
        初始化互斥量，restrict为C中的关键字，表示无法修改该指针的值
        attr 互斥量属性 一般为NULL

    int pthread_mutex_destroy(pthread_mutex_t * mutex);
        销毁互斥量

    int pthread_mutex_lock(pthread_mutex_t * mutex);
        加锁（如已有锁，会阻塞）
    int pthread_mutex_trylock(pthread_mutex_t * mutex);
        尝试加锁（如有锁，会返回）
    int pthread_mutex_unlock(pthread_mutex_t * mutex);
        解锁
*/

// 案例：三个线程销售100张票

#include <stdio.h>
#include <pthread.h>
// #include <>

// 临界资源在共享  全局区
int tickets = 100;

pthread_mutex_t mutex;


void *selltickets(){

    while(1){
        // 对临界区加锁，这里不能在while前面加锁，
        // 这样会让一个线程把所有的临界资源消耗完
        pthread_mutex_lock(&mutex);
        if(tickets > 0){
            printf("%ld卖出了第%d张票\n",pthread_self(),tickets);
            tickets --;
        }else{
        //如果资源已经没有了，在退出循环前解锁 
            pthread_mutex_unlock(&mutex);
            break;
        }
        // 如果有资源，则在这里解锁，不让其一直占用资源
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(){

    pthread_mutex_init(&mutex,NULL);
    pthread_t tid_1;
    pthread_t tid_2;
    pthread_t tid_3;

    pthread_create(&tid_1,NULL,selltickets,NULL);
    pthread_create(&tid_2,NULL,selltickets,NULL);
    pthread_create(&tid_3,NULL,selltickets,NULL);

    pthread_join(tid_1,NULL);
    pthread_join(tid_2,NULL);
    pthread_join(tid_3,NULL);

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}



