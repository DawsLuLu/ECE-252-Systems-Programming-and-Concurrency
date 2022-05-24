/**
 * Socket client sends 1 MiB transfer to server.  TCP layer delivers in
 * mulitple smaller segments.
 */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	// lookup server address
	struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
	struct addrinfo *server;
	getaddrinfo("localhost", "2520", &hints, &server);

	// create socket
	int sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	// connect to server
	connect(sockfd, server->ai_addr, server->ai_addrlen);

	// send data
	const size_t bufSize = 1*1024*1024; // 1 MiB
	uint8_t buf[bufSize];
	ssize_t bytes = send(sockfd, buf, bufSize, 0);
	printf("sent %zd\n", bytes);

	freeaddrinfo(server);
	close(sockfd);

	return 0;
}

