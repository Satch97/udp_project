#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <sys/queue.h>		  /*  lists					    */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <errno.h>

#define MAX_BUF (65507)

#define CLNT_TTL (15)

TAILQ_HEAD(listhead, socket) head;
struct tailhead *headp;

struct socket{
	struct sockaddr_in *sockaddrptr;
	time_t last_acc;
	TAILQ_ENTRY(socket) sockets;
};

int AddSocket(struct sockaddr_in *sock) {
	struct socket *socketentry;
	time_t now = time(NULL);
	struct socket *sockptr;

	for (sockptr = head.tqh_first; sockptr != NULL; sockptr = sockptr->sockets.tqe_next) {
		
		if (sockptr->sockaddrptr->sin_port == sock->sin_port && sockptr->sockaddrptr->sin_addr.s_addr == sock->sin_addr.s_addr) {
			printf("Duplicate found, Age : %ld : Updating Time\n", now - sockptr->last_acc);
			sockptr->last_acc = now;

			return 0;	
		}

	}

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

	memcpy(socketentry->sockaddrptr, sock, sizeof(struct sockaddr_in));
	socketentry->last_acc = now;
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
	unsigned int cl_addr_len = sizeof(cl_addr);
	int len, len2;
	struct socket *sockptr;
	time_t curr_time;

	while(1) {
		// printf("\n");
	    memset(&buffer, 0, sizeof(buffer));
	    memset(&cl_addr, 0, sizeof(cl_addr));
		len = recvfrom(sock, buffer, sizeof(buffer), 0, 
			(struct sockaddr *)&cl_addr, &cl_addr_len);
		curr_time = time(NULL);
		if (len == 1 && buffer[0] == '\n') {
			AddSocket(&cl_addr);
			continue;
		}

		// sleep(1); // delate sending for a second
		for (sockptr = head.tqh_first; sockptr != NULL; sockptr = sockptr->sockets.tqe_next) {
			// printf ("in list : %s, %u\n", inet_ntoa (sockptr->sockaddrptr->sin_addr), ntohs (sockptr->sockaddrptr->sin_port));
			if(curr_time - sockptr->last_acc > CLNT_TTL) {
				TAILQ_REMOVE(&head, sockptr, sockets);
				continue;
			}
			// printf("Sent to a client\n");
			len2 = sendto(sock, buffer, len, 0,
	           	(struct sockaddr *)sockptr->sockaddrptr, sizeof(struct sockaddr));
		}

	}
	return 0;
}