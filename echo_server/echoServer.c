#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#include <pthread.h>

#define MAX_LINE 1024

typedef struct EricConnInfo {
	int fd;
	char* remoteAddr;
} EricConnInfo;

void* echoServerExec(void* connfd);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("[ECHO SERVER] usage echo-server port\n");
		return -1;
	}
	
	printf("[ECHO SERVER] Listen to 127.0.0.1:%s\n", argv[1]);

	int listenfd, connfd;
	struct sockaddr_in sockaddr;
	struct sockaddr_in remoteAddr;
	char buff[MAX_LINE];

	memset(&sockaddr, 0, sizeof(sockaddr));

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(atoi(argv[1]));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bind(listenfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

	listen(listenfd, 1024);

	printf("[ECHO SERVER] Listening..\n");

	while(1) {
		int sinSize;
		sinSize = sizeof(struct sockaddr_in);
		if ((connfd = accept(listenfd, (struct sockaddr*) &remoteAddr, &sinSize)) == -1) {
			printf("[ECHO SERVER] Accept socket error : %s errno :%d\n", strerror(errno), errno);
			continue;
		}
		// create thread to deal with request
		int createThreadReturn;
		pthread_t id;
		EricConnInfo tmp;
		tmp.fd = connfd;
		tmp.remoteAddr = inet_ntoa(remoteAddr.sin_addr);
		createThreadReturn = pthread_create(&id, NULL, echoServerExec, &tmp);
	}
	close(connfd);
	return 0;
}

void* echoServerExec(void* info) {
	int fd = ((EricConnInfo*)info)->fd;
	char* remoteIp = ((EricConnInfo*)info)->remoteAddr;
	char buff[MAX_LINE];
	int n;

	printf("[ECHO SERVER] Get Conn [fd :%d][remote ip : %s]\n", fd, remoteIp);
	if(send(fd, "Welcome\n", 8, 0) < 0) {
		printf("[ECHO SERVER] Response Error : %s errno : %d\n", strerror(errno), errno);
		close(fd);
		return 0;
	}

	while((n = recv(fd, buff, MAX_LINE, 0)) > 0) {
		buff[n] = '\0';
		printf("[ECHO SERVER] From conn [fd : %d]: %s\n", fd, buff);
		if(send(fd, buff,n, 0) < 0) {
			printf("[ECHO SERVER] Response Error : %s errno : %d\n", strerror(errno), errno);
			close(fd);
			return 0;
		}
	}
	printf("[ECHO SERVER] Conn [fd :%d] Close.\n", fd);
	close(fd);
	return 0;
}
