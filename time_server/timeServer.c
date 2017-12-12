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
	if (argc != 2) {
		printf("[TIME SERVER] usage time-server server-port\n");
	}

	// get port info
	int port = atoi(argv[1]);
	printf("[TIME SERVER] Will listen on %d\n", port);

	int fd;
	char buff[BUFFER_SIZE];
	struct sockaddr_in addr;

	// create udp socket
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	bind(fd, (struct sockaddr*) &addr, sizeof(addr));

	// listen
	printf("[TIME SERVER] Listening..\n");

	while(1) {
		int len;
		recvfrom(fd, buff, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &len);
		// cal time
		time_t now;
		time(&now);
		now = htonl((unsigned long) (now + UNIX_EPOCH));
		printf("[TIME SERVER] Recv %s\n", buff);
		sendto(fd, (char*) &now, sizeof(now), 0, (struct sockaddr*) &addr, sizeof(addr));
	}
	close(fd);
	return 0;
}
