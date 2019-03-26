#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <sys/queue.h>		  /*  lists					    */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>



#include <errno.h>

#define MAX_BUF (65507)

TAILQ_HEAD(listhead, socket) head;
struct tailhead *headp;

struct socket{
	struct sockaddr_in *sockaddrptr;
	time_t last_acc;
	TAILQ_ENTRY(socket) sockets;
};

int AddSocket(struct sockaddr_in *sock) {
	struct socket *socketentry;
	socketentry = malloc(sizeof(struct socket));
	if(!socketentry) {
		printf("Error : Malloc error\n");
		return -1;
	}

	socketentry->sockaddrptr = malloc(sizeof(struct sockaddr_in));
	if(!socketentry) {
		printf("Error : Malloc error\n");
		return -1;
	}
	
  	TAILQ_INSERT_TAIL(&head, socketentry, sockets);
  	return 0;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Usage Error : server.exe <server port>");
		exit(EXIT_FAILURE);
	}

	long int rawPort = strtol(argv[1], NULL, 10);
	uint16_t port = (uint16_t) rawPort;

	if(port != rawPort) {
		printf("Error : Invalid port"); // does not fit in range
		exit(EXIT_FAILURE);
	}

	int sock;

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("Error : Socket could not be created"); 
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
	}

	TAILQ_INIT(&head);

	char buffer[MAX_BUF];
	struct sockaddr_in cl_addr;
	unsigned int cl_addr_len;
	int len;
	while(1) {
	    memset(&buffer, 0, sizeof(buffer));
		len = recvfrom(sock, buffer, sizeof(buffer), 0, 
			(struct sockaddr *)&cl_addr, &cl_addr_len);
		printf("buffer : %s\n", buffer);
	}
	return 0;
}