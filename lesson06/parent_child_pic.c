/*分析：
    实现ps aux|grpe xxx
    
    子进程实现ps aux并将结果重定向输出到管道

    父进程从管道读取数据，过滤后输出到终端
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main(){

    int pipefd[2];
    if(pipe(pipefd)==-1){
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if(pid==0){
        // child
        // dp2(fd1,fd2); 将fd2重定向到fd1
        dup2(pipefd[1],STDOUT_FILENO);
        //执行ps aux

        execlp("ps","ps","aux",NULL);

        perror("execlp");
        exit(0);

    }else if(pid > 0){
        // parent
        close(pipefd[1]);

        //读取数据  过滤数据 输出
        char buf[1024]={0};
        //读数据 
        // 因为数据很多 所以循环读完，read会返回读取成功的字节数。
        // 这里的-1 是减掉字符串的结束符。   
        int len = -1;
        while((len = read(pipefd[0],buf,sizeof(buf)-1)) > 0){
            printf("%s",buf);
            memset(buf,0,1024);            
        }
        // 回收子进程资源
        wait(NULL);
    }
    return 0;
}