/**
 * read file with open() and read() system calls and send to stdout
 */

#include <stdio.h> // for printf, perror
#include <stdlib.h> // for malloc, free
#include <unistd.h> // for read, close
#include <string.h> // for memset
#include <fcntl.h> // for open

void readfile( int fd );

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1; // FAILURE
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
		perror("file open failed");
        return 1;
    }

    readfile( fd );
    close( fd );

    return 0; // SUCCESS
}

/**
 * Read file contents and send to stdout.
 */
void readfile(int fd) {
    const size_t buf_size = 256;

	// allocate buffer of buf_size bytes with malloc()
	char *buf;
	buf = malloc(buf_size);

	// loop until end-of-file
    while(1) {
		// clear buf with memset
		memset(buf, 0, buf_size);

		// read buf_size-1 bytes
        ssize_t bytes_read = read(fd, buf, buf_size - 1);  

		// break if end-of-file
		if(bytes_read == 0) break;
		else if(bytes_read == -1) {
			perror("read failed");
			exit(1);
		}

		// display
        printf("%s", buf);
    }

    printf("\nEnd of File.\n");
	free(buf);
}

