#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef struct MSG{
    struct tm *p;
    char message[1024];
    char sender[10];
    char rever[10];
} msg;


int main(){
    int pid=fork();
    if(pid==0){
        while(1){
            sleep(1);
            printf("------\n");
        }
    }else if (pid>0)
    {
        while (1)
        {
            sleep(1);
            printf("++++++++\n");
            /* code */
        }
        
    }
    

}