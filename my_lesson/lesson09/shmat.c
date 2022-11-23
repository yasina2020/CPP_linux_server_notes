/* 共享内存

#include <sys/shm.h>
#include <sys/ipc.h>

int shmget(key_t key,size_t size,int shmflg);
功能：创建一个新的共享内存段（会被初始化为0），或过去一个既有的段标识
key 标识 一般用16进制非0值
size 大小（字节）
shmflg 属性 按位或 |
    访问权限
        0664
    附加属性（创建/判断是否存在/获取）
        IPC_CREAT
        IPC_EXCL 需要和IPC_CREAT一起使用
返回值：成功：>0 共享内存的ID
        失败：-1，设置errno

void *shmat(int shmid,const void *shmaddr,int shmflg);
功能：将进程和共享内存绑定
shmid 共享内存的id
shmaddr NULL 由内核分配进程虚拟空间中的地址
shmflg 该进程对共享内存的操作权限
    执行 SHM_EXEC
    读 SHM_RDONLY
    读写 0
返回值：成功返回共享内存的首地址，失败返回(void *)-1


int shmdt(const void *shmaddr);
功能：解除进程和共享内存的绑定
shmaddr 共享内存地址
返回值：成功0，失败-1

int shmctl(int shmid,int cmd,struct shmid_ds *buf);
功能：操作共享内存
shmid 共享内存标识
cmd
    IPC_STAT 获取SHM的状态
    IPC_SET 设置状态
    IPC_RMID 标记共享内存需要被销毁，标记后，
             当所有的进程都解绑了，共享内存就会被删除
buf 需要设置或获取的共享内存的信息，
    如果是获取的话，buf是传出参数
    删除的话，可为NULL
返回值：成功0，失败-1
struct shmid_ds{
    struct ipc_perm shm_perm; 权限
    size_t          shm_segsz;  段的大小（以字节为单位）
    time_t          shm_atime;  最后一个进程附加到该段的时间
    time_t          shm_dtime;  最后一个进程离开该段的时间
    time_t          shm_ctime;  改变时间
    pid_t           shm_cpid;   创建该段进程的pid*
    pid_t           shm_lpid;   在该段上操作的最后1个进程的pid
    shmatt_t        shm_nattch; 当前附加到该段的进程的个数
    ...
};


key_t ftok(const char *pathname, int proj_id);
根据路径和proj_id生成一个共享内存的key_t
 - key_t int类型 但只用一个字节，所以通常使用一个字符，如'a'

*/