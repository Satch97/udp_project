#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <errno.h>
int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Usage Error : server.exe <server port>");
		exit(EXIT_FAILURE);
	}

	long int rawPort = strtol(argv[1], NULL, 10);
	uint16_t port = (uint16_t) rawPort;

	if(port != rawPort) {
		printf("Invalid port"); // does not fit in range
		exit(EXIT_FAILURE);
	}

	int sock;

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("Error creating socket"); 
		exit(EXIT_FAILURE);
	}

	stuct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = htonl(INADDR_ANY);

	return 0;
}