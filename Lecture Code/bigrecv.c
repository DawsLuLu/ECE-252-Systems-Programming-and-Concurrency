/**
 * Socket server receives 1 MiB transfer from client.  Comes in multiple
 * smaller segments.
 */
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	// create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// enable quick re-use of server port
	int flag = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	// bind socket to port 2520
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(2520),
		.sin_addr.s_addr = htonl(INADDR_ANY) };
	bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

	// listen for connect() requests
	listen(sockfd, 0);

	// accept connect() request
	int newsockfd = accept(sockfd, NULL, NULL);

	// receive data
	const size_t bufSize = 2*1024*1024; // 2 MiB
	uint8_t buf[bufSize];
	size_t rcvd = 0, space = bufSize;
	do {
		ssize_t bytes = recv(newsockfd, buf + rcvd, space, 0);
		printf("bytes %zd\n", bytes);
		if(bytes <= 0) break;
		rcvd += bytes;
		space -= bytes;
	} while(space > 0);

	printf("rcvd %zu\n", rcvd);

	close(newsockfd);
	close(sockfd);

	return 0;
}

