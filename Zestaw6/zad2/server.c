#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>

mqd_t klienci[10];
mqd_t s;

void nowy_klient(char *kom)
{
	for (int i=0; i<10; i++)
	{
		if (klienci[i]==-1)
		{
			char name[6];
			sprintf(name,"%s",kom+1);
			if ((klienci[i]=mq_open(name,O_WRONLY))<0)
			{
				perror("Otwarcie kolejki klienta");
				exit(EXIT_FAILURE);
			}
			char ii[1024];
			sprintf(ii,"%i",i);
			if ((mq_send(klienci[i],ii,1024,1))<0)
			{
				perror("Wysłanie identyfikatora do klienta");
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
}

void kasuj_klienta(char *kom)
{
	int id=(int)kom[1];
	if ((mq_close(klienci[id]))<0) perror("Zamknięcie kolejki klienta przez serwer");
	klienci[id]=-1;
}

void koniec()
{
	for (int i=0; i<10; i++)
	{
		if (klienci[i]!=-1)
		{
			if ((mq_close(klienci[i]))<0) perror("Zamknięcie kolejki klienta przez serwer na końcu");
		}
	}
	if ((mq_close(s))<0) perror("Zamknięcie kolejki serwera przez serwer");
	if ((mq_unlink("/sq"))<0) perror("Usunięcie kolejki serwera");
}

void sigint(int sig)
{
	exit(EXIT_SUCCESS);
}

void czas(int id)
{
	time_t t;
	time(&t);
	if ((mq_send(klienci[id],ctime(&t),1024,1))<0)
	{
		perror("Wysłanie czasu do klienta");
		exit(EXIT_FAILURE);
	}
}

void mirror(char *kom)
{
	int id=(int)kom[1];
	char kom2[1024];
	sprintf(kom2,"%s",kom+2);
	int d=strlen(kom2)-1;
	char t[d];
	for (int i=0; i<d; i++)
	{
		t[i]=kom2[d-1-i];
	}
	if ((mq_send(klienci[id],t,1024,1))<0)
	{
		perror("Wysłanie lustra do klienta");
		exit(EXIT_FAILURE);
	}
}

void calc(char *kom)
{
	int id=(int)kom[1];
	int a=kom[2]-48;
	int b=kom[4]-48;
	char t[1024];
	if (kom[3]=='+')
	{
		int w=a+b;
		sprintf(t,"%i",w);
	}
	if (kom[3]=='*')
	{
		int w=a*b;
		sprintf(t,"%i",w);
	}
	if (kom[3]=='-')
	{
		int w=a-b;
		sprintf(t,"%i",w);
	}
	if (kom[3]=='/')
	{
		int w=a/b;
		sprintf(t,"%i",w);
	}
	if ((mq_send(klienci[id],t,1024,1))<0)
	{
		perror("Wysłanie wyniku do klienta");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char const *argv[])
{
	atexit(koniec);
	signal(SIGINT,sigint);
	for (int i=0; i<10; i++)
	{
		klienci[i]=-1;
	}
	if ((s=mq_open("/sq",O_RDONLY | O_CREAT,0666,NULL))<0)
	{
		perror("Stworzenie kolejki serwera");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		char kom[9000];
		if ((mq_receive(s,kom,9000,NULL))<0)
		{
			perror("Odebranie od klienta");
			exit(EXIT_FAILURE);
		}
		if (kom[0]=='I') nowy_klient(kom);
		if (kom[0]=='T') czas((int)kom[1]);
		if (kom[0]=='S') kasuj_klienta(kom);
		if (kom[0]=='E') exit(EXIT_SUCCESS);
		if (kom[0]=='M') mirror(kom);
		if (kom[0]=='C') calc(kom);
	}
}