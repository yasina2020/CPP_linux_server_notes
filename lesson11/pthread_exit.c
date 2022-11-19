/*
#include <pthread.h>
    
    void pthread_exit(void *retval);
        终止调用该函数的线程，
        retval ：需要传递一个指针，作为返回值，可以在pthread_join()中获取

    int pthread_join(pthread_t thread, void **retval);
    和一个已经退出的线程连接
        用来回收子线程的资源（阻塞，类似于wait(),
        调用一次回收一个子线程，一般在子线程中使用）
    tread 子线程ID
    retval 二级指针，接收子线程退出时的返回值，不需要接收返回值时，置为 NULL。
    返回值：
        成功返回0 
        失败返回非0的错误宏 <errno.h> 
            EDEADLK 死锁
            EINVAL 1、目标线程本身不允许其它线程获取它的返回值，
                   2、事先就已经有线程调用 pthread_join() 函数获取到了目标线程的返回值
            ESRCH 找不到指定的 thread 线程
    注：一个线程执行结束的返回值只能由一个 pthread_join() 函数获取，阻塞
        线程结束后资源不会释放，只有在其他线程中调用 pthread_join() 来帮助释放。

    
    pthread_t pthread_self(void);
        返回当前线程的线程ID
    int pthread_equal(pthread_t t1,pthread_t t2);
        比较两个线程ID是否相等
*/  

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


typedef struct STU{
    int id;
    char *name;
}stu;

void *my_call(void * arg){
    stu *s = (stu *)arg;
    printf("this is child thread id:%ld...\n",pthread_self());
    // stu *s = (stu *)malloc(sizeof(stu));
    s->id=10;
    s->name="Jane";
    // pthread_exit((void *)&s);
    return NULL;
}

int main(){
    pthread_t tid;
    stu s1;
    s1.id=20;
    s1.name="zhangsan";
    printf("befor:id:%d,name:%s\n",s1.id,s1.name);

    int ret = pthread_create(&tid,NULL,my_call,&s1);
    if(ret != 0){
        printf("error:%s\n",strerror(ret));
    }

    printf("父线程id:%ld子线程id:%ld\n",pthread_self(),tid);

    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }

    // void * s;

    // pthread_join(tid,&s);
    pthread_join(tid,NULL);
    printf("after:id:%d,name:%s\n",s1.id,s1.name);

    // printf("id:%d,name:%s\n",((stu*)s)->id,((stu*)s)->name);

    pthread_exit(NULL);

    return 0;

}
