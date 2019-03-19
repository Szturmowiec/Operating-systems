#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int state=1;

void ctrlc()
{
	printf("%s\n","Odebrano sygnał SIGINT");
	exit(0);
}

void ctrlz()
{
	if (state==1)
	{
		state=0;
		printf("%s\n","Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu");
	}
	else state=1;
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
	while(1)
	{
		if (state==1)
		{
			time_t t;
    		time(&t);
    		printf("\n%s",ctime(&t));
			sleep(1);
		}
		else sleep(1);
	}
	return 0;
}