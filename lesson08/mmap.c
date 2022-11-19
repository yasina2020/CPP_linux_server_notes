/* 
#inlcude <sys/mman.h>
void *mmap(void*addr,size_ length,int port,int flags,int fd,off_t offset);
        将一个文件或者设备中的数据映射到内存中
    addr 要映射到内存中的地址，NULL 由内核决定
    length 要映射的数据的长度（不能为0，建议使用文件的长度 stat或lseek获取长度）
            如果数据大小不够内存中的一页的大小，则分配页的整数倍大小。
    prot 对内存映射区的操作权限（PROT_READ|PROT_WRITE）
            PROT_EXEC 执行
            PROT_READ 读（必须有）
            PROT_WRITE 写
            PROT_NONE 无
    flags 
        MAP_SHARED 映射区修改后，会同步到磁盘文件中，进程通信中设置
        MAP_PRIVATE 映射区修改后，磁盘文件不变，会重新创建一个新的文件，写时copy
    fd:磁盘文件的描述符（通过open得到，文件要大于0）
        open时的权限和port的权限不能有冲突（即open的权限要大于port的权限）
    offset：磁盘文件偏移量（4k的整数倍），0表示不偏移，有时候我们并不想从文件一开始映射

    返回值：返回内存首地址，失败返回 MAP_FAILED  其实是(void*)-1


int munmap(void *addr.size_t length); 
    释放映射的内存。
    addr 内存地址
    length 要释放的内存大小,和mmap一样
*/
/* 
内存映射区  非阻塞
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int main(){
    char *file = "mmapfile.txt";
    // 文件不存在就创建，并且因为mmap的size大小不能为0，所以要往里面写一些数据。
    // 如果mmap写入的数据大于文件的大小，就截取
    if(access(file,F_OK)==-1){
        printf("文件不存在，创建\n");
        // 赋予权限
        int fd = creat(file,S_IRWXU|S_IRWXG|S_IRWXO);
        if(fd == -1){
            perror("create");
            exit(0);
        }
        char buf[128]={0};
        if(write(fd,buf,sizeof(buf)) ==-1){
            perror("write");
            exit(0);
        }
    }
    int fd = open("mmapfile.txt",O_RDWR);
    if(fd==-1){
        perror("open mmapfile.txt");
    }
    struct stat statbuff;
    stat("mmapfile.txt",&statbuff);
    // int size = lseek(fd,0,SEEK_END);
    int size = statbuff.st_size;
    printf("%d,%d\n",size,fd);
    void *addr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(addr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //在父进程映射之后再创建子进程，这个时候子进程会共享父进程的映射区。
    pid_t pid = fork();
    if(pid>0){
        strcpy((char*)addr,"hello son!");
    }else if (pid==0){
        char buf[128];
        strcpy(buf,(char*)addr);
        printf("read data:%s\n",buf);
    }
    // 解除映射
    munmap(addr,size);
    return 0;
}

