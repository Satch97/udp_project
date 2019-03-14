#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage Error : client.exe <server IP> <server port>");
		exit(EXIT_FAILURE);
	}

	char *addr = argv[1];

	long int rawPort = strtol(argv[2], NULL, 10);
	uint16_t port = (uint16_t) rawPort;

	if(port != rawPort) {
		printf("Invalid port"); // does not fit in range
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htnos(port);

	if (inet_aton (addr, &sockaddr.sin_addr) <= 0) {
        printf("Error : Invalid Address\n");
        exit(EXIT_FAILURE);
    }

	int sock;
	sock = socket (PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("Error : Socket could not be created\n");
	}
}