/*
线程不属于系统库，属于第三方库，所以在编译的时候需要链接第三方库
如： gcc pthread_create.c -o app -pthread
或：gcc pthread_create.c -o app -lpthread


#include <pthread.h>
int pthread_create(pthread_t *thread,const pthread_attr_t *attr,
                    void *(*start_routine) (void *),void *arg);
    创建一个子线程
    - thread:传出参数，保存创建成功后的子线程的ID
    - attr 需要设置的县城属性，默认值NULL
    - start_routine 函数指针，是子线程需要处理的代码
    - arg 给第三个参数传参
    返回值成功0，失败返回错误号 
        获取错误信息：char * sererror(int errnum);

*/ 

#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>


void *my_call(){

    printf("this is thread....\n");
    return NULL;
}
int main(){
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,my_call,NULL);
    if(ret != 0){
        printf("pthread_create ERROR:%s\n",strerror(ret));
    }

    for(int i=0;i<5;i++){
        printf("%d\n",i);
    }

    sleep(1); // 为了防止主线程在子线程没有运行完时退出，

    return 0;
}