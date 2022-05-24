#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	int fd = open("example.txt", O_RDWR);

	struct stat st;
	stat("example.txt", &st);
	ssize_t size = st.st_size;
	void *mapped = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	switch(fork()) {
	case -1:
		fprintf(stderr, "fork error\n");
		return 1;
	case 0: // child
	{
		memset(mapped, 0, size);
		sprintf(mapped, "It is now overwritten");
		msync(mapped, size, MS_SYNC);
		munmap(mapped, size);
		break;
	}
	default: // parent
		wait(NULL);
		printf("New file content: %s\n", (char *)mapped);
		munmap(mapped, size);
	}

	return 0;
}
