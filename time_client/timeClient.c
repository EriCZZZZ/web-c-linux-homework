#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<time.h>

#define BUFFER_SIZE 1024
#define UNIX_EPOCH 2208988800UL

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("[TIME CLIENT] usage time-client server-ip server-port\n");
		return 0;
	}
	// get server info
	char* ip = argv[1];
	int port = atoi(argv[2]);
	printf("[TIME CLIENT] Connect to time server [ip : %s][port : %d]\n", ip, port);

	int fd;
	char buff[BUFFER_SIZE];
	struct sockaddr_in addr;

	// create a udp socket
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// init socketaddr
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);

	// send service info
	sendto(fd, "time", sizeof("time"), 0, (struct sockaddr*) &addr, sizeof(addr));

	int len;
	time_t now;
	len = read(fd, (char *)&now, sizeof(now));
	now = ntohl((unsigned long) now);
	now -= UNIX_EPOCH;
	printf("[TIME CLIENT] CURRENT TIME : %s", ctime(&now));

	close(fd);

	return 0;
}

