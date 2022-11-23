/*


线程属性

typedef struct
{
       int __detachstate;// 分离状态
       int __schedpolicy;//调度算法
       struct sched_param __schedparam;//线程的调度参数
       int __inheritsched;//线程的继承性
       int __scope;//线程的作用域
       size_t __guardsize;//线程栈末尾的警戒缓冲区大小
       int __stackaddr_set;//线程的栈设置
       void* __stackaddr;//线程栈的位置
       size_t __stacksize;//线程栈的大小
} pthread_attr_t;

pthread_attr_t p_attr;
使用此变量前，必须调用 pthread_attr_init() 函数进行初始化为默认属性
pthread_arrt_init(&p_attr); // 成功0 失败非0

int pthread_attr_init(pthread_attr_t *attr);
// 初始化为默认属性
int pthread_attr_destroy(pthread_attr_t *attr);
// 销毁线程属性资源，
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
// 获取分离状态 
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
// 设置分离状态 
。。。。。
所有的属性都有设置和过去该属性的两个调用函数。

__detachstate 属性值用于指定线程终止执行的时机，该属性的值有两个，分别是：
                PTHREAD_CREATE_JOINABLE:（默认值）线程执行完函数后不会自行释放资源；
                PTHREAD_CREATE_DETACHED:线程执行完函数后，会自行终止并释放占用的资源。
__schedpolicy 属性用于指定系统调度该线程所用的算法，它的值有以下 3 个：
                SCHED_OTHER:（默认值）分时调度算法；
                SCHED_FIFO:先到先得（实时调度）算法；
                SCHED_RR:轮转法；
__scheparam 用于设置线程的优先级（默认值为 0），仅当__schedpolicy 属性不为SCHED_OTHER时才能发挥作用。



*/