#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>

char **buf;
sem_t sem;
sem_t full;
sem_t empty;

int ip=0;
int ik=0;
int ile=0;
int p;
int k;
int n;
int nk;
int l;
pthread_t *tp;
pthread_t *tk;
char tryb;
char name[20];
char info[4];

void term(int sig)
{
	for (int i=0; i<k; i++) pthread_cancel(tk[i]);
	for (int i=0; i<p; i++) pthread_cancel(tp[i]);
	sem_destroy(&sem);
	sem_destroy(&full);
	sem_destroy(&empty);
	exit(EXIT_SUCCESS);
}

void config(char *conf)
{
	FILE *f=fopen(conf,"r");
	char *b=(char*)malloc(30*sizeof(char));
	while ((fgets(b,30,f))!=NULL)
	{
		int i=0;
		int ai=0;
		char *a=(char*)malloc(30*sizeof(char));
		while (b[i]!='=') i++;
		i++;

		while (b[i]!=' ' && b[i]!='\n' && b[i]!='#' && b[i]!='\r')
		{
			if (b[i]!=39 && b[i]!='"')
			{
				a[ai]=b[i];
				ai++;
			}
			i++;
		}

		switch(b[0])
		{
			case 'P':
				p=atoi(a);
				break;
			case 'K':
				k=atoi(a);
				break;
			case 'N':
				n=atoi(a);
				break;
			case 'n':
				if (b[1]=='k') nk=atoi(a);
				else strcpy(name,a);
				break;
			case 'L':
				l=atoi(a);
				break;
			case 't':
				tryb=*a;
				break;
			case 'i':
				strcpy(info,a);
				break;
		}
	}
}

void *producent(void *f)
{
	while(1)
	{
		if (strcmp(info,"full")==0) printf("Producent nr %lu czeka na wolne miejsce\n",pthread_self());
		sem_wait(&empty);
		sem_wait(&sem);

		char *b=(char*)malloc(100*sizeof(char));
		if (strcmp(info,"full")==0) printf("Producent nr %lu produkuje linię\n",pthread_self());
		if ((fgets(b,100,f))==NULL)
		{
			sem_post(&sem);
			break;
		}

		if (buf[ip]==NULL)
		{
			buf[ip]=(char*)malloc(100*sizeof(char));
			buf[ip]=b;
			ile++;
		}
		if (strcmp(info,"full")==0) printf("Producent nr %lu zapisał produkt i kończy cykl produkcji\n",pthread_self());
		ip++;
		if (ip==n) ip=0;

		sem_post(&full);
		sem_post(&sem);
	}
	pthread_exit(0);
}

void *konsument(void *f)
{
	while(1)
	{
		if (strcmp(info,"full")==0) printf("Konsument nr %lu czeka na produkt\n",pthread_self());
		sem_wait(&full);
		sem_wait(&sem);

		char *b=(char*)malloc(100*sizeof(char));
		if (buf[ik]!=NULL)
		{
			b=buf[ik];
			buf[ik]=NULL;
			ile--;
		}
		if (strcmp(info,"full")==0) printf("Konsument nr %lu skonsumował\n",pthread_self());

		if (tryb=='<' && b!=NULL)
		{
			if (strlen(b)-1<l) printf("Napis o id %d to %s",ik,b);
		}

		if (tryb=='>' && b!=NULL)
		{
			if (strlen(b)-1>l) printf("Napis o id %d to %s",ik,b);
		}

		if (tryb=='=' && b!=NULL)
		{
			if (strlen(b)-1==l) printf("Napis o id %d to %s",ik,b);
		}

		ik++;
		if (ik==n) ik=0;
		sem_post(&empty);
		sem_post(&sem);
	}
	pthread_exit(0);
}

int main(int argc,char const *argv[])
{
	tp=(pthread_t*)malloc(p*sizeof(pthread_t));
	tk=(pthread_t*)malloc(k*sizeof(pthread_t));

	char *conf=(char*)argv[1];
	config(conf);

	sem_init(&sem,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,n);

	if (nk==0) signal(SIGINT,term);
	else signal(SIGINT,SIG_IGN);

	FILE *f=fopen(name,"r");
	buf=(char**)malloc(n*sizeof(char*));
	for (int i=0; i<p; i++)
	{
		pthread_create(&tp[i],NULL,&producent,f);
	}
	for (int i=0; i<k; i++)
	{
		pthread_create(&tk[i],NULL,&konsument,f);
	}

	signal(SIGALRM,term);
	if (nk>0) alarm(nk);
	for (int i=0; i<p; i++)
	{
		pthread_join(tp[i],NULL);
	}

	if (nk==0) term(0);

	for (int i=0; i<k; i++)
	{
		pthread_join(tk[i],NULL);
	}
}