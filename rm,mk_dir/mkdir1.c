//system call mkdir()을 통해서 만든 mkdir1

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        if (mkdir(argv[i], 0777) < 0)	//path(argc[i])로 지정한 디렉터리를 만든다. 성공하면 반환, 실패하면 -1 반환하고 errno 설정
			  // 두번 재 인지는 mode로써 권한을 지정한다. mode와 unmask의 &연산으로 나오는 결과값이 권한이 지정된다! (자세한 건 노트정리 확인)
		  {
            perror(argv[i]);
            exit(1);
        }
    }
    exit(0);
}
