/*
读写锁

读时共享，写时独占
写锁高优先级

pthread_rwlock_t 
读写锁的数据类型
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
销毁读写锁
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
           const pthread_rwlockattr_t *restrict attr);
初始化读写锁

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
加  读锁
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
尝试加读锁
pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
加  写锁
pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
尝试加写锁
pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
解锁

案例：两个线程对临界资源写，五个线程对临界资源读。
这个感觉有bug

*/


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define _XOPEN_SOURCE 500

int num = 100;

pthread_rwlock_t rwlock;

void *my_call_w(){
    int cnt = 20;
    while(cnt--){
        
        pthread_rwlock_wrlock(&rwlock);
        usleep(5);
        printf("tid:%ld,修改了num:%d\n",pthread_self(),num);
        num++;
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}


void *my_call_r(){
    int cnt = 10;
    while(cnt--){
        pthread_rwlock_rdlock(&rwlock);
        sleep(1);
        printf("tid:%ld,读取了num:%d\n",pthread_self(),num);
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

int main(){

    pthread_rwlock_init(&rwlock,NULL);
    pthread_t rtid[5],wtid[2];
    for(int i =0;i<2;i++){
        pthread_create(&wtid[i],NULL,my_call_w,NULL);
    }
    for(int i =0;i<5;i++){
        pthread_create(&rtid[i],NULL,my_call_r,NULL);
    }

    for(int i =0;i<2;i++){
        pthread_detach(wtid[i]);
    }
    for(int i =0;i<5;i++){
        pthread_detach(rtid[i]);
    }

    pthread_exit(NULL); // 主线程退出后，后面的代码就不会执行了。

    printf("main pthread end-------\n");
    pthread_rwlock_destroy(&rwlock);

    return 0;

}

