#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define MAXLINE 1024

/**
 * echo-client host-ip host-port
 * 
 */
int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage : echo-client host-ip host-port\n");
		return -1;
	}
	printf("[ECHO CLIENT] CONNECT to %s:%s\n", argv[1], argv[2]);

	char *servIp = argv[1];
	int socketfd;
	struct sockaddr_in sockaddr;
	char recvline[MAXLINE], sendline[MAXLINE];
	int n;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family=AF_INET;
	// sockaddr.sin_addr.s_addr=inet_addr(servIp);
	sockaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, servIp, &sockaddr.sin_addr);

	// connect
	if ((connect(socketfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr))) < 0) {
		printf("[ECHO CLIENT] Connect error %s errno: %d\n", strerror(errno), errno);
		return -1;
	}

	printf("[ECHO CLIENT] CONNECTED\n");
	
	n = recv(socketfd, recvline, MAXLINE, 0);
	recvline[n] = '\0';
	printf("[ECHO CLIENT] Received %s\n", recvline);

	while(1) {
		printf("[ECHO CLIENT] Please input something and press ENTER:");
		scanf("%s", sendline);
		if (strcmp("exit", sendline) == 0) {
			break;
		}

		if ((send(socketfd, sendline, strlen(sendline), 0)) < 0) {
			printf("[ECHO CLIENT] Send msg error: %s errno : %d", strerror(errno), errno);
		} else {
			n = recv(socketfd, recvline, MAXLINE, 0);
			recvline[n] = '\0';
			printf("[ECHO CLIENT] Received: %s\n", recvline);
		}
		memset(sendline, 0, sizeof(char) * MAXLINE);
	}

	close(socketfd);
	printf("[ECHO CLIENT] EXIT\n");
	return 0;
}
