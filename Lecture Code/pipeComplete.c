#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 25

int main(int argc, char *argv[]) {
	char write_msg[BUF_SIZE] = "Greetings";
	char read_msg[BUF_SIZE];

	// create pipe
	int fd[2];
	if(pipe(fd) == -1) {
		perror("pipe failed");
		return 1;
	}

	switch(fork()) {
		case -1:
			perror("fork failed");
			return 1;

		case 0: // child
			// close read end
			close(fd[0]);
			// write message
			write(fd[1], write_msg, strlen(write_msg) + 1);
			// close write end
			close(fd[1]);
			break;

		default: // parent
			// close write end
			close(fd[1]);
			// read message and print
			read(fd[0], read_msg, BUF_SIZE);
			printf("%s\n", read_msg);
			// close read end
			close(fd[0]);
			break;
	}

	return 0;
}

