/**
 * Practice with fork(), exec() and wait().
 * - Try replacing execl() with printf().
 * - Try adding a global variable, increment in child, and print in
 *   parent.
 */

#include <stdio.h> // perror, printf
#include <stdlib.h> // exit
#include <sys/wait.h> // wait
#include <unistd.h> // execlp

int main(int argc, char *argv[]) {
	switch(fork()) {

		case -1: // error
			perror("fork failed");
			return 1;

		case 0: // child
			execlp("ls", "ls", "-a", NULL);

		default: { // parent
			int status;
			wait(&status);
			printf("Child returned %d.\n", status);
		}
	}

	return 0;
}

