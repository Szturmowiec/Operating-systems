#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int sig,siginfo_t *t,void *x)
{
		printf("%d\n",t->si_value.sival_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    sigset_t set;
    sigfillset(&set);
    sigaddset(&set,SIGUSR2);
    action.sa_mask=set;
    action.sa_flags=SA_SIGINFO;


    //..........


    int child = fork();
    if(child == 0) {
    	sigaction(SIGUSR1,&action,NULL);
    	pause();
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
    }
    else {
    	sleep(1);
    	union sigval t;
    	int w=atoi(argv[1]);
    	int s=atoi(argv[2]);
    	t.sival_int=w;
    	sigqueue(child,s,t);
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
