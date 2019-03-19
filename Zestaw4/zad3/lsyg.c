#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

pid_t child;
pid_t parent;
int ilech;
int ilep;
int type;

void ctrlc()
{
	printf("%s\n","Koniec tego złego");
	if (kill(child,0)==0) kill(child,SIGUSR2);
	exit(0);
}

void child_handle(int sig)
{
	ilech++;
	if (type!=3 && sig==SIGUSR1)
	{
		printf("%s","Potomek otrzymał sygnał SIGUSR1, razem odebranych sygnałów przez potomka: ");
		printf("%d\n",ilech);
		kill(parent,SIGUSR1);
	}
	if (type!=3 && sig==SIGUSR2)
	{
		printf("%s","Potomek otrzymał sygnał SIGUSR2, razem odebranych sygnałów przez potomka: ");
		printf("%d\n",ilech);
		exit(0);
	}

	if (type==3 && sig==SIGRTMIN)
	{
		printf("%s","Potomek otrzymał sygnał czasu rzeczywistego 1, razem odebranych sygnałów przez potomka: ");
		printf("%d\n",ilech);
		kill(parent,SIGRTMIN);
	}
	if (type==3 && sig==SIGRTMIN+1)
	{
		printf("%s","Potomek otrzymał sygnał czasu rzeczywistego 2, razem odebranych sygnałów przez potomka: ");
		printf("%d\n",ilech);
		exit(0);
	}
}

void parent_handle(int sig)
{
	ilep++;
	if (type!=3 && sig==SIGUSR1)
	{
		printf("%s"," Rodzic otrzymał sygnał SIGUSR1, razem odebranych sygnałów przez rodzica: ");
		printf("%d\n",ilep);
	}

	if (type==3 && sig==SIGRTMIN)
	{
		printf("%s"," Rodzic otrzymał sygnał czasu rzeczywistego 1, razem odebranych sygnałów przez rodzica: ");
		printf("%d\n",ilep);
	}
}

int main(int argc,char const *argv[])
{
	int l=atoi(argv[1]);
	type=atoi(argv[2]);

	parent=getpid();
	child=fork();

	if (child>0 && type!=3)
	{
		struct sigaction act;
    	act.sa_handler=parent_handle;
    	sigfillset(&act.sa_mask);
		sigaddset(&act.sa_mask,SIGUSR1);
		sigdelset(&act.sa_mask,SIGUSR2);
		act.sa_flags=0;

		sigaction(SIGUSR1,&act,NULL);
		signal(SIGINT,ctrlc);
		sleep(1);

		for (int i=0; i<l; i++)
		{
			kill(child,SIGUSR1);
			if (type==2) pause();
		}

		kill(child,SIGUSR2);
	}

	if (child==0 && type!=3)
	{
		struct sigaction act;
    	act.sa_handler=child_handle;
    	sigfillset(&act.sa_mask);
		sigaddset(&act.sa_mask,SIGUSR1);
		sigaddset(&act.sa_mask,SIGUSR2);
		act.sa_flags=0;

		sigaction(SIGUSR1,&act,NULL);
		sigaction(SIGUSR2,&act,NULL);

		while(1) pause();
	}

	if (child>0 && type==3)
	{
		struct sigaction act;
    	act.sa_handler=parent_handle;
    	sigfillset(&act.sa_mask);
		sigaddset(&act.sa_mask,SIGRTMIN);
		sigdelset(&act.sa_mask,SIGRTMIN+1);
		act.sa_flags=0;

		sigaction(SIGRTMIN,&act,NULL);
		signal(SIGINT,ctrlc);
		sleep(1);

		for (int i=0; i<l; i++) kill(child,SIGRTMIN);
		kill(child,SIGRTMIN+1);

		while (ilep<l) pause();
	}

	if (child==0 && type==3)
	{
		struct sigaction act;
    	act.sa_handler=child_handle;
    	sigfillset(&act.sa_mask);
		sigaddset(&act.sa_mask,SIGRTMIN);
		sigaddset(&act.sa_mask,SIGRTMIN+1);
		act.sa_flags=0;

		sigaction(SIGRTMIN,&act,NULL);
		sigaction(SIGRTMIN+1,&act,NULL);

		while(1) pause();
	}

	printf("%s","Liczba wysłanych sygnałów do potomka: ");
	printf("%d\n",l+1);
	return 0;
}