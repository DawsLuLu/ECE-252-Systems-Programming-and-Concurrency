/**
 * simple socket client.  Connects to server and receives a message
 * string.
 */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define NOERRORCHECK
#ifdef NOERRORCHECK

int main(int argc, char *argv[]) {

	// lookup server address
	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM };
	struct addrinfo *server;
	getaddrinfo("localhost", "2520", &hints, &server);

	// create socket
	int sockfd = socket(server->ai_family, server->ai_socktype,
			server->ai_protocol);

	// connect to server
	connect(sockfd, server->ai_addr, server->ai_addrlen);

	// receive message from server
	int message = 0;
	recv(sockfd, &message, sizeof(message), 0);
	printf("Client received %d from server.\n", ntohl(message));

	freeaddrinfo(server);
	close(sockfd);

	return 0;
}

#elif

int main(int argc, char *argv[]) {

	// get server address
	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM };
	struct addrinfo *server;
	if(getaddrinfo("localhost", "2520", &hints, &server) != 0) {
		perror("getaddrinfo failed");
		return 1;
	}

	// create socket
	int sockfd = socket(server->ai_family, server->ai_socktype,
			server->ai_protocol);
	if(sockfd == -1) {
		perror("socket failed");
		freeaddrinfo(server);
		return 1;
	}

	// connect to server
	if(connect(sockfd, server->ai_addr, server->ai_addrlen) == -1) {
		perror("connect failed");
		freeaddrinfo(server);
		close(sockfd);
		return 1;
	}

	// receive message from server
	int message;
	if(recv(sockfd, &message, sizeof(message), 0) == -1) {
		perror("recv failed");
		freeaddrinfo(server);
		close(sockfd);
		return 1;
	} else {
		printf("Client received %d from server.\n", ntohl(message));
	}

	freeaddrinfo(server);
	close(sockfd);

	return 0;
}

#endif
