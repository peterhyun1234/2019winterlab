//unlink()를 이용해서 만든 rm 명령어
//파일을 삭제한다는 것은 실체에 붙인 이름이 개수를 줄인다는 뜻이다. 이를 위햇 사용되는 시스템 콜은 unlink()

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
        if (unlink(argv[i]) < 0) {
            perror(argv[i]);
            exit(1);
        }
    }
    exit(0);
}
