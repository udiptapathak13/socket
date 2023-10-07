/*
 * client.c
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
#include <sys/socket.h>
#include <arpa/inet.h>

void talk(int connfd)
{
	int count;
	char buf[20];
	while (count != 1) {
		bzero(buf, sizeof buf);
		read(connfd, buf, sizeof buf);
		count = atoi(buf);
		printf("server: %d\x0a", count);
		count = count & 1 ? (count << 1) + count + 1 : count >> 1;
		printf("client: %d\x0a", count);
		sprintf(buf, "%d", count);
		write(connfd, buf, sizeof buf);
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr)
		== -1) {
		perror("Failed to connect to the server...\x0a");
		return 1;
	}
	printf("Successfully connected to the server..\x0a");
	talk(sockfd);
	close(sockfd);
}
