#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

struct mydata{
	int a;
	int b;
};

int main(int argc, char const *argv[])
{
	// key_t keyval = ftok("/",'A');
    key_t keyval = 101;
	
	int shmid = shmget(keyval, 4096, IPC_CREAT|IPC_EXCL|0777);
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
	if(p == (struct mydata *)(-1))
	{
		perror("shmat");
		return -2;
	}
	


    printf("share mem :pa=%d,pb=%d\n", p->a, p->b);


	shmdt(p);

    // shmctl(shmid,IPC_RMID,NULL);
	return 0;
}

