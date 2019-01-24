// 심볼릭 링크를 만드는 시스템 콜 symlink()로 만든 symlink
// 코드는 하드링크에서 한 줄만 수정했다. 중요한 건  심볼릭 링크에 대한 이해 더 필요

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "%s: wrong number of arguments\n", argv[0]);
        exit(1);
    }
    if (symlink(argv[1], argv[2]) < 0) {
        perror(argv[1]);
        exit(1);
    }
    exit(0);
}
