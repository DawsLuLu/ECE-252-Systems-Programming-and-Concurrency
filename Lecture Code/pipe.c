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

	switch(fork()) {
		case -1:
			perror("fork failed");
			return 1;

		case 0: // child
			// close read end

			// write message

			// close write end

			break;

		default: // parent
			// close write end

			// read message and print


			// close read end

			break;
	}

	return 0;
}

