/**
 * simple socket server.  Waits for client to connect, then sends it a
 * message string.
 */
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define NOERRORCHECK
#ifdef NOERRORCHECK

int main(int argc, char *argv[]) {
	// create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// bind socket to port 2520 (any port > 1023 is okay)
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(2520),
		.sin_addr.s_addr = htonl(INADDR_ANY) };
	bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

	// listen for connect() requests, backlog = 5
	listen(sockfd, 5);

	// accept connect() request
	struct sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr,
			&client_addr_size);

	// send client a message
	int message = htonl(42);
	send(newsockfd, &message, sizeof(message), 0);

	close(newsockfd);
	close(sockfd);

	return 0;
}

#else

int main(int argc, char *argv[]) {
	// create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		perror("socket failed");
		return 1;
	}

	// bind socket to port 2520 (any port > 1023 is okay)
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(2520),
		.sin_addr.s_addr = htonl(INADDR_ANY) };
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind failed");
		close(sockfd);
		return 1;
	}

	// listen for connect() requests - backlog = 5
	if(listen(sockfd, 5) == -1) {
		perror("listen failed");
		close(sockfd);
		return 1;
	}

	// accept connect() request
	struct sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *)&client_addr,
			&client_addr_size);
	if(newsockfd == -1) {
		perror("accept failed");
		close(sockfd);
		return 1;
	}

	// send client a message
	int message = htonl(42);
	if(send(newsockfd, &message, sizeof(message), 0) == -1) {
		perror("send failed");
		close(newsockfd);
		close(sockfd);
		return 1;
	}

	// close client socket
	close(newsockfd);

	// close server socket
	close(sockfd);

	return 0;
}

#endif
