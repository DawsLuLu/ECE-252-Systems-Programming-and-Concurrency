/**
 * Adapted from TLPI Listing 20-1
 * Try Ctrl-C and see what happens.  Use Ctrl-\ to terminate.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigHandler(int sig) {
	printf("Ouch!\n");
}

int main(int argc, char *argv[]) {
	if(signal(SIGINT, sigHandler) == SIG_ERR)
		perror("signal failed");

	int i = 0;
	while(1) {
		printf("%d\n", i++);
		sleep(3);
	}

  return 0;
}

