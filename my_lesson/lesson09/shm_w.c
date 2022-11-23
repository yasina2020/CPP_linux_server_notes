#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

struct mydata{
	int a;
	int b;
};

int main(int argc, char const *argv[]){
	struct mydata data;
    data.a=1;
    data.b=2;

	// key_t keyval = ftok("/",'A');
	key_t keyval = 101;
	int shmid = shmget(keyval,4096,IPC_CREAT | IPC_EXCL | 0777);
	if(shmid < 0)
	{
		if(errno == EEXIST)
		{
			shmid = shmget(keyval,0,0);
		}
		else
		{
			perror("shmget");
			return -1;
		}
	}

	struct mydata *p = shmat(shmid, NULL, 0);
	if(p == (struct mydata*)(-1))
	{
		perror("shmat");
		return -2;
	}

	printf("sizeof(data)=%lu\n", sizeof(data));
	memcpy(p, &data, sizeof(data));  

	shmdt(p);

    getchar();
    shmctl(shmid,IPC_RMID,NULL);
	return 0;
}


