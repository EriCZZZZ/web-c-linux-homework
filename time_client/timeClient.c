#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	if argc != 3 {
		printf("[TIME CLIENT] usage time-client server-ip server-port\n");
	}
	char* ip = argv[1];
	int port = atoi(argv[2]);
	printf("[TIME CLIENT] Connect to time server [ip : %s][port : %d]\n", ip, port);

	int fd;
	char buff[BUFFER_SIZE];
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr=atonl
	return 0;
}

