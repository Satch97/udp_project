#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage Error : client.exe <server IP> <server port>");
		exit(EXIT_FAILURE);
	}

	long int rawPort = strtol(argv[2], NULL, 10);
	uint16_t port = (uint16_t) rawPort;

	if(port != rawPort) {
		printf("Invalid port"); // does not fit in range
		exit(EXIT_FAILURE);
	}
}