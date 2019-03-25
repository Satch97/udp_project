
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF     (65535)
#define STDIN (0)

void InitDefaultTransfer(int sock, struct sockaddr_in sockaddr){
	char nl = '\n';
	while(1) {
		sendto(sock, &nl, 1, 0,
	           (struct sockaddr*)&sockaddr, sizeof(sockaddr));
		sleep(5);
	}
}

void CaptureError(int err, char* failedcall) {
	printf("\nFailed : %s", failedcall);
	printf("Errno is %d\n", errno);
	printf("%s\n", strerror (errno));
	exit(EXIT_FAILURE);
}

void InitDGRead(int sock) {
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	char buffer[MAX_BUF];
	int ret, servaddr_len;
	struct sockaddr_in servaddr;
	while(1) {
		ret = select(sizeof(fds)*8, &fds, NULL, NULL, NULL);
		if (ret == -1) {
			printf("Error : Failure with select");
			exit(1);
		} else if (ret > 0) {
			memset(&buffer, 0, sizeof(buffer));
			recvfrom(sock, buffer, sizeof(buffer), 0,
			(struct sockaddr *)&servaddr, &servaddr_len);
			printf("\n%s\n", buffer);
		}
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

	if (fork() == 0) {
		InitDefaultTransfer(sock, sockaddr);
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN, &fds);
	char buffer[MAX_BUF], c;
	int ret = 0, len = 0, err, bindflag = 1;
	unsigned int servaddr_len;
	struct sockaddr_in servaddr;
	int maxfd = sock > STDIN ? sock : STDIN;

	while(1) {

		printf("\nEnter \'s\' to send a string \nEnter q to exit\nEnter Command : \n");
		ret = select(maxfd + 1, &fds, NULL, NULL, NULL);

		if (ret <= 0) {
			CaptureError(errno, "select");
		} else {
			if (FD_ISSET(STDIN, &fds)){
				memset(&buffer, 0, sizeof(buffer));

                for(len = 0; len < MAX_BUF; len++) {
                	c = getchar();
                	buffer[len] = c;

                	if (c == '\n') break;
            	}

            	if (strcmp(buffer, "s\n") == 0) {
					 printf("\nEnter String : ");
					 memset(&buffer, 0, sizeof(buffer));

					for(len = 0; len < MAX_BUF; len++) {
                		c = getchar();
                		buffer[len] = c;
	                	if (c == '\n'){
	                		break;
	                	}
	                	
            		}

            		sendto(sock, buffer, strlen(buffer), 0,
		            (struct sockaddr*)&sockaddr, sizeof(sockaddr));

				} else {
					printf("Invalid Input\n");
				}

			} 

	        if (FD_ISSET(sock, &fds)){}// read socket
        }	

	}

}