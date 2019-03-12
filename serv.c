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

	return 0;
}