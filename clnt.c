
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF     (65535)

// Heavily inspired from project 1 getinstr function
int GetInstr() {
    char x;
    while(1) {
        printf("Enter \'s\' to send a string \nEnter 'f' to send file\nEnter q to exit\nEnter : ");
        x = getchar();
        getchar(); // get rid of extra newline
        if (x == 's') return 1;
        if (x == 'q') return 0;
    }
}

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage Error : client.exe <server IP> <server port>");
		exit(EXIT_FAILURE);
	}

	char *addr = argv[1];

	long int rawPort = strtol(argv[2], NULL, 10);
	uint16_t port = (uint16_t) rawPort;

	if(port != rawPort) {
		printf("Error : Invalid port"); // does not fit in range
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);

	if (inet_aton (addr, &sockaddr.sin_addr) <= 0) {
        printf("Error : Invalid Address\n");
        exit(EXIT_FAILURE);
    }

	int sock;
	sock = socket (PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("Error : Socket could not be created\n");
	}

	char buffer[MAX_BUF];

	while(1) {

		if(!GetInstr()) {
			return 0;
		}

		memset(&buffer, 0, sizeof(buffer));
		printf("Enter string : ");
        scanf("%s", buffer);
	}

}