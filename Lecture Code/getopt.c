/**
 * demonstrate getopt for parsing command-line arguments
 */

#include <stdbool.h> // for bool
#include <stdio.h> // for printf
#include <stdlib.h> // for EXIT_FAILURE and EXIT_SUCCESS
#include <string.h> // for strncpy
#include <unistd.h> // for getopt

int main( int argc, char *argv[] ) {
	int c; // option character
	int n=0; // n for number
	char fileName[256] = "";
	bool verbose = false;

	// ':' in the optstring indicate that the flag has an argument
	while((c = getopt(argc, argv, "f:n:v")) != -1) {
		switch(c) {
		case 'f':
			strncpy(fileName, optarg, 255);
			fileName[255] = '\0';
			break;
		case 'n':
			n = atoi(optarg);
			break;
		case 'v':
			verbose = true;
			break;
		default:
			return EXIT_FAILURE;
		}
	}

	printf("f=%s, n=%d, v=%s\n", fileName, n, verbose ? "true" : "false");
	return EXIT_SUCCESS;
}
