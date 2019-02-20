#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>


#define Port 9000

char sndBuf[BUFSIZ] = "클라이언트야 잘받았니?";
char rcvBuf[BUFSIZ];
int main()
{

	int c_socket,s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int readlen;

	s_socket = socket(PF_INET, SOCK_STREAM,0); //소켓 생성
	if(s_socket == -1)
	{
		printf("Socket Creation Error\n");
		return -1;
	}

	// <연결 요청을 할 주소 설정> : 소켓 주소 설정
	memset(&s_addr,0,sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //32bit IPV$ 주소
	s_addr.sin_family = AF_INET; //IPV4 인터넷 프로토콜
	s_addr.sin_port = htons(Port); //"Port"를 포트로 사용

	// <소켓을 포트에 연결> : 소켓에 주소 부여
	if(bind(s_socket,(struct sockaddr *)&s_addr, sizeof(s_addr))== -1)
	{
		printf("Bind Error\n");
		return -1;
	}

	// <커널에 개통 요청>
	if(listen(s_socket,5) == -1)
	{
		printf("Listen Error\n");
		return -1;
	}

	while(1){
		// 클라이언트 연결 요청 수신
		printf("Listening....\n\n");
		
		len = sizeof(c_addr);
		// 기다리다가 전화를 받는 함수가 accept, return은 c_socket으로 되는데 전화가 왔을때 다른 소켓으로 만들어서 연결하는 것! 원래있던 s_socket을 듣기소켓으로 쓰인다(연결용으로만), 실질적인 수행은 c_socket!
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr,&len);
		if(c_socket == -1)
		{
			printf("Accept Error\n");
			return -1;
		}

		// <클라이언트 요청 서비스 제공>
		readlen = read(c_socket,rcvBuf,sizeof(rcvBuf));
		if(readlen == -1)
		{
			printf("Read Error\n");
			return -1;
		}
		rcvBuf[readlen] = '\0';	// 클라이언트로부터 받은 데이터의 마지막은 NULL로
		printf("Recieved Data : %s\n",rcvBuf);

		write(c_socket, sndBuf,sizeof(sndBuf));	// 연결된 클라이언트에게 데이터 보내기
		printf("Data being sent : %s\n",sndBuf);


		// 클라이언트와 연결 종료
		close(c_socket);
	}
	// 서버 종료
	close(s_socket);
}


