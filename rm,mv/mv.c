// 파일을 이동하는 mv command
// 파일을 이동한다는 것은 "별도의 하드링크를 만들고 나서 원래 이름을 지운다"는 것이다.
// 명령어로 다시 말하면 "mv a b" == "ln a b" + "rm a"
// 파일 이동하는 API로는 rename()이 있다.

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
    if (rename(argv[1], argv[2]) < 0) {
        perror(argv[1]);
        exit(1);
    }
    exit(0);
}
