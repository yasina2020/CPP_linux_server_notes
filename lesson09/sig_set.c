/*
PCB中有未决信号集和阻塞信号集，是一个64位的二进制数，1表示有信号。
当信号发生时，未决信号集相应位会置一，再去找阻塞信号集中的相应位，如果是0表示非阻塞，那么信号就会产生动作（默认动作，或者回调动作）
    如果为1，就会被阻塞或，不去执行动作，直到变为0.
    默认不阻塞任何信号
我们可以通过系统调用去设置阻塞信号集。

int sigemptyset(sigset_t *set);
    - 清空信号集，置零
    - set 传出参数，是我们要操作的信号集
    - 返回值 成功0，失败-1
int sigfillset(sigset_t *set);
    - 全置1
int sigaddset(sigset_t *set，int signum);
    - 设置信号集中某一个信号为1（阻塞）
int sigdelset(sigset_t *set，int signum);
    - 设置信号集中某一个信号为0（非阻塞）
int sigismember(const sigset_t *set, int signum);
    - 判断某个信号是否阻塞
    - 返回值 被阻塞返回1，未被阻塞返回0，失败返回-1

以上是对自定义信号集操作
以下是对内核PCB中的信号集操作


int sigprocmask(int how,const sigset_t *set,sigset_t *oldset);
    -把自定义信号集设置为内核中的阻塞信号集
    -how：自定义信号集set对内核信号集mask进行的操作
        SIG_BLOCK：将set中的阻塞添加进mask，相当于mask = mask|set
        SIG_UNBLOCK：按照set解除mask中的阻塞，相当于 mask = mask & (~set)
        SIG_SETMASK 用set替换mask
    set：用户自定义的阻塞set
    -oldset 保存的修改前的mask，也可以是NULL
    返回值 成功0 失败-1

int sigpending(sigset_t *set);
    获取内核中的未决信号集
    set 传出参数
    返回值 成功0 失败-1


*/