// 인자가 필요한 간단한 ls
// 디렉터리도 파일처럼 open -> read -> close

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>	//for opendir(),readdir(),closedir()

static void do_ls(char *path);

int
main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        do_ls(argv[i]);
    }
    exit(0);
}

static void
do_ls(char *path)
{
    DIR *d;
    struct dirent *ent;

    d = opendir(path);	// 디렉토리 열고
    if (!d) {	//path가 없거나 NULL이 반환된 경우 exit()
        perror(path);
        exit(1);
    }
    while (ent = readdir(d)) {		// 더 읽어 들일 엔트리가 없을 때까지(NULL 반환될 때까지) readdir반복
        printf("%s\n", ent->d_name);	// struct dirent는 리눅스에서는 char* d_name이 있고 이건 기본적으로는 NULL을 담고 있는 문자열이다. 
    }
    closedir(d);	// 디렉토리 열었으니까 다시 닫고
}
