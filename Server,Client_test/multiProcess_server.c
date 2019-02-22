// 여러 클라이언트가 서버에 접속할때 처리하기 위한 구조

// 부모 프로세스는 접수담당, 자식은 입출력(전송)담당해서 분업하는 구조

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void errProc();
void errPrint();
int main(int argc, char*  argv[])
{
	int srvSd, clntSd;
	struct sockaddr_in srvAddr, clntAddr;
	int clntAddrlen, readlen;
	char rBuff[BUFSIZ];
	pid_t pid;

	if(argc != 2){
		printf("Usage: %s [port]\n", argv[0]);
		exit(1);
	}
	printf("Server start...\n");

	srvSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	//클라이언트의 접속요청을 처리할 소켓생성
	if(srvSd == -1) errProc();

	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(atoi(argv[1]));

	if (bind(srvSd,(struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
	errProc();
	if(listen(srvSd, 5) <0)
	errProc();


	clntAddrlen = sizeof(clntAddr);

	while(1)
	{
		clntSd = accept(srvSd, (struct sockaddr *)&clntAddr, &clntAddrlen);
		if(clntSd == -1){
		errPrint();
		continue;
		}

		printf("client %s:%d is connected...\n", inet_ntoa(clntAddr.sin_addr),ntohs(clntAddr.sin_port));
		pid = fork();	//자식 프로세스를 생성, 자식프로세스는 듣기 소켓을 종료하고 입출력을 처리한다.
		if(pid == 0)	//child process
		{
			close(srvSd);
			while(1)
			{
				readlen = read(clntSd, rBuff, sizeof(rBuff)-1);
				if(readlen == 0) break;
				rBuff[readlen] = '\0';
				printf("Client(%d) : %s\n", ntohs(clntAddr.sin_port), rBuff);
				write(clntSd, rBuff, strlen(rBuff));
			}
			printf("Client(%d) : is disconnected\n", ntohs(clntAddr.sin_port));
			close(clntSd);
			return 0;
		}
		else if(pid == -1) errProc("fork");
		
		else	//Parent process : 부모 프로세스는 연결 소켓을종료하고 다음 accept를 처리할 준비를 한다. 
		{
			close(clntSd);
		}
	}
	close(srvSd);
	return 0;
}

void errProc(const char *str)
{
	fprintf(stderr, "%s: %s \n",str, strerror(errno));
	exit(1);
}

void errPrint(const char *str)
{
	fprintf(stderr,"%s: %s\n", str, strerror(errno));
}

		

