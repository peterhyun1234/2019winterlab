//talk_client_pthread.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *do_keyboard(void *);
void *do_socket(void *);

char quit[ ] = "exit";

pthread_t pid[2];

int
main(int argc, char *argv[ ]) {

	int	connSock;
	struct sockaddr_in server_addr;
	char	*serverAddr;
	int	serverPort;
 
	int	thr_id, status;
	int	len;

	if(argc != 3) {
		printf("Usage:  talk_client server_ip port_num ");
		return -1;
	} else {
		serverAddr = argv[1];
		serverPort = atoi(argv[2]);
	}

	if((connSock=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Talk Client Can't open socket\n");
		return -1;
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverAddr);
	server_addr.sin_port = htons(serverPort);

	if(connect(connSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("talk Client Can't connect \n");
		return -1;
	}
	printf("Talk Client connect to Talk server \n");

	thr_id=pthread_create(&pid[0], NULL, do_keyboard, (void *)connSock);
	thr_id=pthread_create(&pid[1], NULL, do_socket, (void *)connSock);

	pthread_join(pid[0], (void **) &status);
	pthread_join(pid[1], (void **) &status);

	close(connSock);
}

void *
do_keyboard(void *data)
{
	int	n;
	char	sbuf[BUFSIZ];
	int	connSock = (int) data;

	while((n = read(0, sbuf, BUFSIZ)) > 0) {
		if(write(connSock, sbuf, n) != n) {
			printf("Talk Server fail in sending\n");
		}

		if(strncmp(sbuf, quit, 4) == 0) {
			pthread_kill(pid[1],SIGINT);
			break;
		}
	}
}

void *
do_socket(void *data)
{
	int 	n;
	char	rbuf[BUFSIZ];
	int	connSock = (int) data;

	while(1) {
		if((n = read(connSock, rbuf, BUFSIZ)) > 0) {
			rbuf[n] = '\0';
			printf("%s", rbuf);

			if(strncmp(rbuf, quit, 4) == 0) {
				pthread_kill(pid[0],SIGINT);
				break;
			}
		}
	}

}

