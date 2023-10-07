/*
 * server.c
 *
 * This file is part of socket/tcp project authored by Udipta Pathak
 * (udiptapathak00@gmail.com)
 *
 * Source code may be used and modified by anyone to produce their work in any
 * form, given that due credits are given to the author of this project in that
 * work.
 *
 * Refer to the license attached to this project's root as well.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>

void talk(int connfd)
{
	int count = 1 + rand() % 100;
	char buf[20];
	while (count != 1) {
		usleep(500000);
		count = count & 1 ? (count << 1) + count + 1 : count >> 1;
		printf("server: %d\x0a", count);
		bzero(buf, sizeof buf);
		sprintf(buf, "%d", count);
		write(connfd, buf, 20);
		read(connfd, buf, 20);
		count = atoi(buf);
		printf("client: %d\x0a", count);
	}
}

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1) {
		perror("Failed to create a socket...\x0a");
		return 1;
	}
	printf("Successfully created a socket..\x0a");
	struct sockaddr_in servaddr = {};
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(14234);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr)
		== -1) {
		perror("Failed to bind socket to an address...\x0a");
		return 1;
	}
	printf("Successfully bound socket to an address..\x0a");
	if (listen(sockfd, 1) == -1) {
		perror("Failed to listen for connections...\x0a");
		return 1;
	}
	printf("Successfully listening to connections..\x0a");
	struct sockaddr_in cliaddr;
	socklen_t clilen;
	int connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
	if (connfd == -1) {
		perror("Failed to accept a connection...\x0a");
		return 1;
	}
	printf("Successfully accepted a connection..\x0a");
	talk(connfd);
	close(connfd);
	close(sockfd);
}
