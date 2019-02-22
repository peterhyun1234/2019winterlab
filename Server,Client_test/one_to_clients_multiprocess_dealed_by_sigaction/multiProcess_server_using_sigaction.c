// 멀티 프로세스를 이용해서 만든 서버에서 시그널을 처리하는 것을 더했다.(좀비 프로세스 생성 방지)
// 앞선 코드와 동일한 부분은 생략하겠다.


main(int argc, char* argv[])
{
	struct sigaction act;

	act.sa_handler = sigHandler; //시그널을 처리해줄 핸들러 지정
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	state = sigaction(SIGCHILD,&act,0);

	while(1)
	{
		//생략
		if(pid =fork())<0)
		{
			printf("fork error");
			return -1;
		}
		else if(pid > 0)	//parent
		{
			close(clntSd);
			continue;
		}
		else if(pid == 0)
		{
			printf("child create.");
			close(srvSd);
			// service part

			close(clntSd);
			return 0;



		}
	}

}

void sigHandler(int sig)
{
	int pid;
	int status;

	pid = wait(&status);
	printf("pid[%d] terminate",pid);

}
