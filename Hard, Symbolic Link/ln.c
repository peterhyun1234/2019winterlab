// 하드링크를 작성하는 시스템 콜 link()로 link만들기
//	link()는 중요한 condition들이 있다. 
// 첫번째 인자인 src와 두번째 인자인 dest가 동일한 파일 시스템에 있어야한다.
// src와 dest에 디렉터리는 사용할 수 없다. (디렉터리에 하드링크를 붙일 수 없다.)



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "%s: wrong arguments\n", argv[0]);
        exit(1);
    }
    if (link(argv[1], argv[2]) < 0) {
        perror(argv[1]);
        exit(1);
    }
    exit(0);
}
