#include <stdio.h>
#include <stdlib.h>
// (1) getopt_long()을 사용하기 위한 정의
#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

// (2) 긴 옵션을 위해 struct option의 배열을 만들고 긴옵션에 해당하는 요소를 정의, 마지막은 모든 요소를 0으로 해야함 
static struct option longopts[] = {
    {"lines", required_argument, NULL, 'n'},
    {"help",  no_argument,       NULL, 'h'},
    {0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
    int opt;
    long nlines = DEFAULT_N_LINES;

    // (3) 옵션을 파싱하는 코드 롱 옵션 --가 아니라 -로 시작하는 짧은 옵션 지정
    while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'n':         // (4) -n, --lines옵션에 대한 처리
            nlines = atol(optarg);
            break;
        case 'h':         // (5) help옵션을 처리한다 usage에 대한 내용을 얼마나 보여줄 지는 개발자 맘대로
            fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(0);
        case '?':         // (6) 모르는 옵션을 받았을때 에러출력
            fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
            exit(1);
        }
    }
    // (7) 
    if (optind == argc) {
        do_head(stdin, nlines);
    } else {
        int i;

        /* (7') */
        for (i = optind; i < argc; i++) {
            FILE *f;

            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_head(f, nlines);
            fclose(f);
        }
    }
    exit(0);
}

static void
do_head(FILE *f, long nlines)
{
    int c;

    if (nlines <= 0) return;
    while ((c = getc(f)) != EOF) {
        if (putchar(c) < 0) exit(1);
        if (c == '\n') {
            nlines--;
            if (nlines == 0) return;
        }
    }
}
