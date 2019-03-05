#include stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX 512

char	quit[ ] = "exit";
int	numClient=0;

main(int argc, char *argv[ ])
{
	int	connSock, listenSock;
	struct sockaddr_in s_addr, c_addr;
	int	len, i, n;
	char	rcvBuffer[BUFSIZ], sbuf[BUFSIZ];
	int	maxfd;

	if(argc < 2) {
		printf("Usage: talk_server port_num");
		return -1;
	}

	fd_set read_fds;
	28 
	listenSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(argv[1]));

	if(bind(listenSock, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}

	if(listen(listenSock, 1) == -1) {
		printf("listen Fail\n");
		return -1;
	}

	connSock=accept(listenSock, (struct sockaddr *) &c_addr, &len);

	while(1) {
		maxfd = connSock + 1;		// select구조체의  0~2번은 에러나, std입출력처리 + 
		FD_ZERO(&read_fds);	//0으로 초기화
		FD_SET(0, &read_fds);	//stdin 초기화 (키보드)
		FD_SET(connSock, &read_fds);

		if(select(maxfd, &read_fds, NULL, NULL, NULL) < 0) {
			printf("select error\n");
			exit(-1);
		}

		printf("server waiting---\n");
		if(FD_ISSET(0, &read_fds)) {	//키보드에서 사건이 발생했는지 check
			if((n = read(0, sbuf, BUFSIZ)) > 0) {		//stdin에서 읽어서
					if(write(connSock, sbuf, n) != n) {	//소켓에 쓰고 전송
						printf("Talk Server fail in sending\n");
					}
					if(strncmp(sbuf, quit, 4) == 0) {
						break;
					}
			}
		}
		if(FD_ISSET(connSock, &read_fds)) {	//소켓에서 사건이 발생했는지 check
			if((n=read(connSock, rcvBuffer,sizeof(rcvBuffer)))!=0){	//리시브 버퍼서에서  읽어서  
				rcvBuffer[n]='\0';
				printf("receive[%s]\n", rcvBuffer);	//받은 data 출력
			}
			if(strncmp(rcvBuffer, quit, 4) == 0) {
				break;
			}
		}
	}
}
