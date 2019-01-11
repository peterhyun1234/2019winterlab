#include <stdio.h>

#include <unistd.h> //For read(), write()
#include <sys/types.h>	//for open()
#include <sys/stat.h>	//for open()
#include <fcntl.h>	//for open()
#include <string.h>	//for memset()

#define MAX 1024

int main(int argc, char *argv[])
{
	int fd;
	int i;
	char buf[MAX];
	int n;

	fd = open("Test.t",O_RDONLY);

	printf("fd is %d\n",fd);

	printf("<Intial BUf>\n%s\n",buf);
	memset(buf,0x00,MAX);

	printf("<After Memset>\n%s\n",buf);
	n = read(fd,buf,MAX-1);  //fd가 바라보는 파일에서 len 길이만큼 buf에 읽어 들임
	printf("Readn = %d\n",n);
	printf("\n<After Reading>\n%s\n", buf);

	if(close(fd)<0)
	{
	
	}
}
