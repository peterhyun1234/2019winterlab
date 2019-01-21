// grep이 해당하는 줄 전체 를 출력하는 것이라면 slice는 해당하는 문자열만 출력하는 명령어

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_slice(regex_t *pat, char* argv[], FILE *f);

int main(int argc, char *argv[])
{
	regex_t pat;
	int err;
	int i;

	if (argc <2){
		fputs("no pattern\n",stderr);
		exit(1);
	}
	err = regcomp(&pat, argv[1],REG_EXTENDED | REG_NEWLINE); //grep에서 REG_NOSUB를 제외하고 컴파일 -> NOSUB flag가 설정된 경우는 regexec에서 pmatch 인수를 무시하기 때문에 regeexec에서 옵션들을 사용하기 위해서는 NOSUB 제외하고 컴파일! 

	if (err!=0)
	{
		char buf[1024];
		regerror(err, &pat, buf, sizeof buf);
		puts(buf);
		exit(1);
	}
	if(argc==2)
	{
		do_slice(&pat, argv, stdin);
	}
	else
	{
		for (i=2; i<argc; i++)
		{
			FILE *f;

			f = fopen(argv[i],"r");

			if(!f)
			{
				perror(argv[i]);
				exit(1);
			}
			do_slice(&pat,argv, f);
			fclose(f);
		}
	}
	regfree(&pat);
	exit(1);

}


static void do_slice(regex_t *pat,char *argv[], FILE *src)
{
	char buf[1024];

	while(fgets(buf, sizeof buf, src))
	{

		if(regexec(pat,buf,0,NULL,0) == 0)
		{
			fputs(argv[1],stdout);
			fputc('\n',stdout);
		}
	}

}

