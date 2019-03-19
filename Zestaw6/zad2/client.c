#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char kom2[9000];
mqd_t s;
mqd_t k;
char name[5];

void term()
{
	char st[1024];
	st[0]='S';
	st[1]=kom2[0];
	if ((mq_send(s,st,1024,1))<0)
	{
		perror("Wysłanie zlecenia STOP");
		exit(EXIT_FAILURE);
	}
	if ((mq_close(k))<0) perror("Zamknięcie kolejki klienta przez klienta");
	if ((mq_close(s))<0) perror("Zamknięcie kolejki serwera przez klienta");
	if ((mq_unlink(name))<0) perror("Usunięcie kolejki klienta");
}

void sigint(int sig)
{
	exit(EXIT_SUCCESS);
}

int main(int argc,char const *argv[])
{
	atexit(term);
	signal(SIGINT,sigint);
	sprintf(name,"/%i",getpid());
	if ((s=mq_open("/sq",O_WRONLY))<0)
	{
		perror("Otworzenie kolejki serwera");
		exit(EXIT_FAILURE);
	}
	if ((k=mq_open(name,O_RDONLY | O_CREAT,0666,NULL))<0)
	{
		perror("Stworzenie kolejki klienta");
		exit(EXIT_FAILURE);
	}

	char kom[1024];
	kom[0]='I';
	sprintf(kom+1,"%s",name);
	if ((mq_send(s,kom,1024,1))<0)
	{
		perror("Wysłanie klucza do serwera");
		exit(EXIT_FAILURE);
	}

	if ((mq_receive(k,kom2,9000,NULL))<0)
	{
		perror("Odebranie identyfikatora od serwera");
		exit(EXIT_FAILURE);
	}

	int mircal=0;
	FILE *f=fopen("zlecenia","r");
	char line[100];
	while ((fgets(line,100,f))!=NULL)
	{
		char zl[1024];
		if ((strcmp(line,"TIME\n"))==0)
		{
			zl[0]='T';
			zl[1]=(char)kom2[0]-'0';
			if ((mq_send(s,zl,1024,1))<0)
			{
				perror("Wysłanie zlecenia TIME");
				exit(EXIT_FAILURE);
			}
		}
		if ((strcmp(line,"END\n"))==0)
		{
			zl[0]='E';
			if ((mq_send(s,zl,1024,1))<0)
			{
				perror("Wysłanie zlecenia END");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		if (line[0]=='M' && line[1]=='I' && line[2]=='R' && line[3]=='R' && line[4]=='O' && line[5]=='R')
		{
			mircal=1;
			zl[0]='M';
			zl[1]=(char)kom2[0]-'0';
			sprintf(zl+2,"%s",line+7);
			if ((mq_send(s,zl,1024,1))<0)
			{
				perror("Wysłanie zlecenia mircalROR");
				exit(EXIT_FAILURE);
			}
		}
		if (line[0]=='C' && line[1]=='A' && line[2]=='L' && line[3]=='C')
		{
			mircal=1;
			zl[0]='C';
			zl[1]=(char)kom2[0]-'0';
			sprintf(zl+2,"%s",line+5);
			if ((mq_send(s,zl,1024,1))<0)
			{
				perror("Wysłanie zlecenia CALC");
				exit(EXIT_FAILURE);
			}
		}

		char odp[9000];
		if ((mq_receive(k,odp,9000,NULL))<0)
		{
			perror("Odebranie odpowiedzi od serwera");
			exit(EXIT_FAILURE);
		}
		printf("%s",odp);
		if (mircal==1)
		{
			mircal=0;
			printf("\n");
		}
	}
	exit(EXIT_SUCCESS);
}