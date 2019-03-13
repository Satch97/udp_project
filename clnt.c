#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage Error : client.exe <server IP> <server port>");
		exit(EXIT_FAILURE);
	}
}