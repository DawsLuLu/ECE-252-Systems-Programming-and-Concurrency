/**
 * @author Andrew Morton
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main( int argc, char *argv[] ) {

	int shmid = shmget(IPC_PRIVATE, 32, IPC_CREAT | 0600);
	void *mem;

	switch(fork()) {
	case -1:
		perror(NULL);
		return EXIT_FAILURE;

	case 0: // child
		mem = shmat(shmid, NULL, 0);
		memset(mem, 0, 32);
		sprintf(mem, "Hello World");
		shmdt(mem);
		break;

	default: // parent
		wait(NULL);
		mem = shmat(shmid, NULL, 0);
		printf("Message from child proc: %s\n", (char *)mem);
		shmdt(mem);
		shmctl(shmid, IPC_RMID, NULL);
		break;

	}

	return EXIT_SUCCESS;
}

