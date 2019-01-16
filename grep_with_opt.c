// grep1.c에서 -I옵션과 -v옵션을 추가
// -I 옵션은 문자열을 대소문자 구분하지 않음 (--ignore-case)
// -v 옵션은 선택된 문자열이 제외된 라인들을 출력 (--invert-match)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h> 		//Regulation Expression
// regcomp, regfree, regexec, regerror

static void do_grep(regex_t *pat, FILE *f);

int
main(int argc, char *argv[])
{
    regex_t pat;	//정규표현식 문자열을 전용 데이터 형식으로 변환(By regcomp)한 후 넣을 변수
    int err;
    int i;

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }
    err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);	//두 번째 인자로 넘어온 정규표현식 argv[1]를 전용데이터 형식인 regex_t로 변환한다. 변환결과는 regex_t type인 pat에 기록된다.
	 // 이때 첫 번째 인자인 pat의 메모리 영역은 호출하기 전에 할당하여 그 포인터를 전달해야 하는데, 그외에도 regcomp()가 독자적으로 메모리를 확보하게 된다. 동적할당이니 free필요하겠지? free의 동작은 regfree로!


    if (err != 0) {	//regcomp가 제대로 수행되면 0을 반환한다. 따라서 0이 아니면 errorcode가 반환된다. 이 code를 regerror에 인자로 넣으면 어떤 error인지 출력한다.
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (argc == 2) {
        do_grep(&pat, stdin);	//출력창에서 입력한 정규식이 있는지 확인
    }
    else {	//지정한 파일이 있는 경우
        for (i = 2; i < argc; i++) {
            FILE *f;
            
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);	//regcomp가 독자적으로 확보한 메모리인 pat를 해제
    exit(0);
}
