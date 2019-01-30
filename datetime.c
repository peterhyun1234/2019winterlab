// datatime이라는 서비스의 클라이언트를 만들어봄
// 소켓으로 접속하면 서버가 현재 시각을 반환해준다
// 클라이언트는 read()하여 표시할 뿐이다.
// $ ./datetime localhost  <= 이런식으로 사용 ㄱㄱ


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static int open_connection(char *host, char *service);	// TCP 접속으로 담당하는 함수
// 첫 번째 인자는 호스트명, 두 번째 인자로는 접속할 서비스(프로토콜 명)를 전달한다.


int
main(int argc, char *argv[])
{
    int sock;
    FILE *f;
    char buf[1024];

    sock = open_connection((argc>1 ? argv[1] : "localhost"), "daytime");
    // open_connection으로 이미 스트림을 얻었기 때문에 이제 스트립에서 fgets()로 한줄씩 읽어서 출력하면 된다.
	 f = fdopen(sock, "r");
    if (!f) {
        perror("fdopen(3)");
        exit(1);
    }
    fgets(buf, sizeof buf, f);
    fclose(f);
    fputs(buf, stdout);
    exit(0);
}

static int
open_connection(char *host, char *service)
{
    int sock;
    struct addrinfo hints, *res, *ai;
    int err;

	 // getaddrinfo의 호출코드 7줄
	 // hints는 반환하는 IP주소의 후보를 줄이기 위해 지정한다.
	 // 여기에서 사용되는 hints는 AF_UNSPEC, SOCK_STREAM으로 각각의 의미는 다음과 같다.
	 // AF_UNSPEC : Address Family를 지정하지 않음 => IPv4,IPv6상관 x
	 // SOCK_STREAM : 패킷이 아니라 스트림의 접속을 사용한다. 즉, TCP를 의미한다.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }

	 // getaddrinfo()로 얻은 값들을 socket, connect로 넘겨주는 구조
	 // 여기서 for문을 잘보면 알 수 있듯이 getaddrinfo()의 반환값은 링크드 리스트 형식이다.

    for (ai = res; ai; ai = ai->ai_next)	//소켓을 접속하는 반복문 
	 {
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock < 0) {
            continue;
        }
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        /* success */
        freeaddrinfo(res);
        return sock;
    }
    fprintf(stderr, "socket(2)/connect(2) failed");
    freeaddrinfo(res);
    exit(1);
}
