/*
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
    - 给 pid进程或者进程组 发送信号sig（编号或者宏值）
    -pid >0  进程的pid
         =0  发送给调用这个函数的进程组中的每一个进程
         =-1 发送给每一个有权限接收这个信号的进程
         <-1 绝对值是进程组的id，发给这个进程组中的每一个进程
    -sig =0 不发送
            发送sig

int raise(int sig);
    - 给当前进程发送信号
    - sig：要发送的信号
    返回值：成功返回0，失败返回非0

void abort(void);
    - 发送SIGABRT信号给当前进程
    - SIGABRT：杀死当前进程

#include <unistd.h>
unsigned int alarm(unsigned int seconds);
-设置定时器，时间结束时会给当前进程发送SIGALRM信号
-seconds 定时器时长，为0时，无效或者取消该定时器。
返回值： 之前没有定时器 返回0，
        之前有定时器，返回上一个定时器剩余的时间。
SIGALRM 默认终止当前进程，每个进程仅有唯一的定时器。

#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_val,
                struct itimerval *old_value);
设置定时器（精度比alarm高），可以周期性的定时，
即执行该函数后，在new_val->it_value后发送which信号，之后每隔new_val—>it_interval时间就发送一次which
    参数：
        which參数表示类型。可选的值有：
            ITIMER_REAL:以系统真实的时间来计算，它送出SIGALRM信号。
            ITIMER_VIRTUAL：以该进程在用户态下花费的时间来计算，它送出SIGVTALRM信号。
            ITIMER_PROF：以该进程在用户态下和内核态下所费的时间来计算。它送出SIGPROF信号。
        new_val：
            new_val->it_value:设置延时时间，到达后发出信号（which选择），
                                且将值变为new_val—>it_interval,重新执行。
            new_val—>it_interval：相当于设置间隔时间
        old_value：用来记录上一次发送信号时的new_val的值，一般设置为NULL，
注意：设置初始值时，new_val->it_value为0，不会触发信号
      new_val->it_value不为0，new_val—>it_interval为0，只会触发一次

struct itimerval{
    struct timeval it_interval; 
    struct timeval it_value;  
}
struct timeval {
    time_t      tv_sec;  秒       
    suseconds_t tv_usec;   微秒    
}


信号捕捉函数
    个人感觉就想中断处理函数，将某个函数和某个信号绑定到一起，当发出该信号时，就执行这个函数。
#include <signal.h>

sighandler_t signal(int signum,sighandler_t handler);
- 捕获某个信号后执行的动作
- sugnum 要捕获的信号
- handler： SIG_IGN 忽略
            SIG_DEF 执行该信号默认动作
            回调函数：执行回调函数
返回值： 成功时返回上一次捕获的回调函数的地址，第一次捕获时返回NULL
        失败时返回SIG_ERR
注：SIGKILL和SIGSTOP无法被捕获或者忽略
    回调函数形式：void (*sig_hander_t)(int)   int 是捕捉到的信号的值
    用例：void my_call(int signum){
        DoSomeThing
    }


int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
    为指定信号改变或检查处理动作
    - signum:要捕捉的信号   注：SIGKILL和SIGSTOP无法被捕获或者忽略
    - act:捕捉到信号后的处理动作
    - oldact：传出参数，上一次信号捕捉时设置的动作，一般为NULL
    返回值 成功0 失败-1

struct sigaction {
    //回调函数1，和singal()中的一样
    void     (*sa_handler)(int);
    // 回调函数2，一般使用
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    // 设置临时的阻塞信号集，回调函数执行结束后，恢复成原来的
    sigset_t   sa_mask;
    // 0：使用回调函数1，SA_SIGINFO：使用回调函数2，更多见man手册
    int        sa_flags;
    // 已废弃
    void     (*sa_restorer)(void);
};


注：SIGKILL和SIGSTOP无法被捕获或者忽略

*/

#include<stdio.h>
#include<unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void my_call(int num){
    printf("这是回调函数，信号是：%d\n",num);
}
//signal信号捕捉函数
void signal_signal_func(){
    signal(SIGALRM,my_call);
}

//sigaction信号捕捉函数
void signal_sigaction_func(){
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = my_call;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM,&act,NULL);

}

int main(){

    signal_sigaction_func();
    struct itimerval it;
    it.it_interval.tv_sec=0;
    it.it_interval.tv_usec=500000;
    it.it_value.tv_sec=1;
    it.it_value.tv_usec=0;
    
    setitimer(ITIMER_REAL,&it,NULL);
    while(1);
    

    return 0;
}