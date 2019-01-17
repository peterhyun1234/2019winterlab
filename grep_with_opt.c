// grep1.c에서 -I옵션과 -v옵션을 추가
// -I 옵션은 문자열을 대소문자 구분하지 않음 (--ignore-case)
// -v 옵션은 선택된 문자열이 제외된 라인들을 출력 (--invert-match)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h> 		//Regulation Expression
// regcomp, regfree, regexec, regerror
#include <unistd.h>	//for getopt()


static void do_grep(regex_t *pat, FILE *f,int invert_mode);

int
main(int argc, char *argv[])
{
    regex_t pat;	//정규표현식 문자열을 전용 데이터 형식으로 변환(By regcomp)한 후 넣을 변수
    int err;
    int i;

	 int opt;
	 int V_mode = 0;
	 int cnt = 0;
	 int flag;	 //regcomp의 인자인 flag로 I 옵션 처리가능

	 //EX == 1
	 //NL == 4
	 //ICASE == 2
	 //NOSUB == 8



	 // getopt은 호출될 때 인자로 넘겨진 뒤 옵션 문자를 반환한다.
	 // 잘못된 옵션이 발견된 경우는 '?'를 반환
	 // 모든 옵션을 반환하면 -1 반환. 따라서 -1까지 while loop
	 // 옵션 뒤에 인자를 받지 않으면 "Iv"라고 하면되는데 순서 상관 x
	 // 옵션 뒤에 인자를 받으면 옵션문자뒤에 ":"를 붙이면된다.
	 while((opt = getopt(argc, argv, "Iv"))!=-1)
	 {
		 switch(opt){
			 case 'I':
				 // -I옵션을 처리하는 코드

				 cnt = 1;
				 flag = REG_ICASE;
				 break;
			 case 'v':
				 // -v옵션을 처리하는 코드
				 V_mode = 1;
				 cnt = 1;
				 break;
			 case '?':
				 // 잘못된 옵션을 처리하는 코드
				 fprintf(stderr, "Usage: %s [-Iv] [<file>..]\n",argv[0]);

				 exit(1);
		 }
	 }


	 //옵션들을 처리하는 방법(관련 전역 변수들)
	 // 1) optarg : 현재 처리중인 옵션의 파라미터
	 // 2) optind : 현재 처리중인 옵션의 argv 인덱스
	 // 3) optopt : 현재 처리중인 옵션 문자
	 // 4) opterr : 이 값이 '참'이면 getopt()가 에러 메시지를 표시
	 
	 










    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }
    err = regcomp(&pat, argv[1+cnt], REG_EXTENDED | REG_NOSUB | REG_NEWLINE |flag);	//두 번째 인자로 넘어온 정규표현식 argv[1]를 전용데이터 형식인 regex_t로 변환한다. 변환결과는 regex_t type인 pat에 기록된다.
	 // 이때 첫 번째 인자인 pat의 메모리 영역은 호출하기 전에 할당하여 그 포인터를 전달해야 하는데, 그외에도 regcomp()가 독자적으로 메모리를 확보하게 된다. 동적할당이니 free필요하겠지? free의 동작은 regfree로!


    if (err != 0) {	//regcomp가 제대로 수행되면 0을 반환한다. 따라서 0이 아니면 errorcode가 반환된다. 이 code를 regerror에 인자로 넣으면 어떤 error인지 출력한다.
        char buf[1024];

        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }

    if ((argc==2)||((argc==3)&&(cnt==1)))	// 지정된 파일이 없을 경우 
	 {
        do_grep(&pat, stdin, V_mode);	//출력창에서 입력한 정규식이 있는지 확인
    }

    else {	//지정한 파일이 있는 경우
        for (i = 2+cnt; i < argc; i++) {
            FILE *f;
            
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f, V_mode);
            fclose(f);
        }
    }
    regfree(&pat);	//regcomp가 독자적으로 확보한 메모리인 pat를 해제
    exit(0);
}



static void
do_grep(regex_t *pat, FILE *src, int invert_mode)	//이 함수가 호출되는시점에 정규표현식은 regex_t형식으로 변환되어 있음.
{
    char buf[4096];

    while (fgets(buf, sizeof buf, src)) {	//src에서 한 줄씩 읽으면서
        if ((regexec(pat, buf, 0, NULL, 0) == 0)&&!(invert_mode)) 
			  fputs(buf, stdout);	//해당하는 값을 찾으면 출력한다.

		  if ((regexec(pat,buf,0,NULL,0)!=0)&&invert_mode)	//V_mode가 참이고  해당하는 패턴이 없을 때를 출력하면 invert mode
			  fputs(buf, stdout);
				  
        
    }
}
