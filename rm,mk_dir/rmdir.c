//system call rmdir()로 빈 디렉토리를 ㅇ삭제하는 명령어 rmdir

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        if (rmdir(argv[i]) < 0)	// rmdir을 성공하면 0을 반환하고 실패하면 -1 반환, 그에 따른 errno 설정
		  {
            perror(argv[i]);
            exit(1);
        }
    }
    exit(0);
}
