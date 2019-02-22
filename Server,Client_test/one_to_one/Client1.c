#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define Port 9000

int main()
{
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	int n;

	char rcvBuf[BUFSIZ];
	char sndBuf[BUFSIZ]="서버야 잘 받았어?";
	c_socket = socket(PF_INET,SOCK_STREAM,0);

	memset(&c_addr,0,sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(Port);


	// <소켓을 서버에 연결> : 여기에서 connect는  전화걸기 함수
	if(connect(c_socket,(struct sockaddr *)&c_addr, sizeof(c_addr)) == -1)
	{
		printf("Connect Error\n");
		close(c_socket);
		return -1;
	}

	// < 서비스 요청과 처리> : 원격지에서 오는 데이터를 보내거나 읽는다(server1에서는 "Hello world"를 write했으므로 헬로우 월드를 rcvBuf에 읽어온다)
	write(c_socket, sndBuf,sizeof(sndBuf));
	printf("Data being sent : %s\n",sndBuf);
	
	if((n = read(c_socket,rcvBuf,sizeof(rcvBuf))) < 0)
	{
		printf(" Read Error\n");
		return -1;
	}
	
	rcvBuf[n] = '\0';
	printf("Received Data : %s\n", rcvBuf);


	// 소켓 연결 종료
	close(c_socket);
}
