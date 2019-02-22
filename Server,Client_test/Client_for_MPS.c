//만들어 놓은 멀티 프로세스 서버에 이용할 수 있는 클라이언트이다.

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


void errProc();

int main(int agrc, char* argv[])
{
	int clntSd;
	struct sockaddr_in clntAddr;
	int clntAdrrlen, readlen, recvByte, maxBuff;
	char wBuff[BUFSIZ];
	char rBuff[BUFSIZ];
	if(argv != 3)
	{
		printf("Usage: %s[IP Address] [Port]\n", argv[0]);
	}
	clntSd = socket(AF_INET,SOCK_STREAM, 0);
	if(clntSd ==-1)errProc();

	printf("==== client program ====\n");

	memset(&clntAddr, 0, sizeof(clntAddr));
	clntAddr.sin_family = AF_INET;
	clntAddr.sin_addr.s_addr = inet_addr(argv[1]);
	clntAddr.sin_port =htons(atoi(argv[2]));

	if(connect(clntSd, (struct sockaddr *)&clntAddr, sizeof(clntAddr)) == -1)
	{
		close(clntSd);
		errProc();
	}


	// <서비스 제공 영역> : 루프를 돌면서 사용자의 문자열을 입력받아 서버로 전송한다. "END"를 입력받으면 루프를 빠져나온다.
	while(1)
	{
		fgets(wBuff, BUFSIZ-1, stdin);	//stdin에서 읽어서 wBuff로
		readlen = strlen(wBuff);
		if(readlen < 2) continue;	//readlen이 2보다 작으면 다시 입력받는다.(while문 바로 뒤 라인)
		write(clntSd,wBuff,readlen-1);	// 입출력용 소켓에 입력한 내용 삽입
		maxBuff = BUFSIZ-1;
		
		//이미 서버가 전송할 문자열의 크기를 알고 있기때문에 그 크기만큼 수신할때 까지 루프를 수행한다.
		do{
			recvByte += read(clntSd, rBuff, maxBuff);
			maxBuff -= recvByte;
		}while(recvByte < (readlen -1));



		rBuff[recvByte] ='\0';
		printf("Sever: %s\n", rBuff);
		wBuff[readlen -1] = '\0';
		if(!strcmp(wBuff,"END")) break;
	}
	printf("END!!^^\n");
	close(clntSd);

	return 0;
}

void errProc()
{
	fprintf(stderr, "Error: %s\n", strerror(errno));
	exit(errno);
}

