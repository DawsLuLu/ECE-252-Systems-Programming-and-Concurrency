/**
 * demonstrate error handling
 */

#include <errno.h> // for errno
#include <stdio.h> // for perror
#include <string.h> // for strerror
#include <unistd.h> // for read

#define BUF_SIZE 16

int main( int argc, char *argv[] ) {
	char buf[BUF_SIZE];
	if(read(-1, buf, BUF_SIZE) == -1) { // forcing error
		perror("read failed");
		printf("\n... or you can do this ...\n\n");
		printf("%s:%d:read failed: %s\n", __FILE__, __LINE__, strerror(errno));
	}
}
