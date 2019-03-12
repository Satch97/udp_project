#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

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

	struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
		printf("Error : Binding Failure");
		exit(EXIT_FAILURE);
	};

	return 0;
}