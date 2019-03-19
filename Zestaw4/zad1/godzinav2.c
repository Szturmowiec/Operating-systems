#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

pid_t child;

void ctrlc()
{
	printf("%s\n","Odebrano sygnał SIGINT");
	if (kill(child,0)==0) kill(child,SIGKILL);
	exit(0);
}

void ctrlz()
{
	if (kill(child,0)==-1)
	{
		child=fork();
		if (child==0) execl("./data.sh","data.sh",NULL);
	}
	else
	{
		kill(child,SIGKILL);
		printf("%s\n","Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu");
		int st;
		waitpid(child,&st,0);
	}
}

void (*signal(int signo,void (*func)()))();

int main(int argc,char const *argv[])
{
	struct sigaction act;
	act.sa_handler=ctrlz;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	sigaction(SIGTSTP,&act,NULL);
	signal(SIGINT,ctrlc);

	child=fork();
	if (child==0) execl("./data.sh","data.sh",NULL);
	while (1) pause();
	return 0;
}