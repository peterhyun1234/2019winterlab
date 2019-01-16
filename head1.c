// 간단하게 만든 head명령어 
//다음과 같이 사용가능하다.

// $cat args.c | ./head1 5

#include <stdio.h>
#include <stdlib.h>

static void do_head(FILE *f, long nlines);

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }
    do_head(stdin, atol(argv[1]));	//문자열을 long타입으로 변환하는 함수
	 // 비슷한건 atoi() > in타입으로 변환
    exit(0);
}

static void
do_head(FILE *f, long nlines)	//첫번째 인자는 읽어 들이는 대상 스트립, 두 번째는 출력할 라인 수
{
    int c;

    if (nlines <= 0) return;
    while ((c = getc(f)) != EOF) {
        if (putchar(c) < 0) exit(1);
        if (c == '\n') {
            nlines--;	//첫 번째 줄부터 출력할 라인 수까지 출력하고 종료하는 함수
            if (nlines == 0) return;
        }
    }
}
