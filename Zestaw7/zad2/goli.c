#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

struct buf
{
    pid_t pid;
    int sem;
};

struct ultimate
{
    int sleepp;
    int ilefree;
    pid_t strz;
    sem_t klienci[50];
    int top_klienci;
    struct buf fifo[50];
    int top_fifo;
};

struct ultimate* u;
sem_t* ifsleep;
sem_t* klient;
sem_t* przedwyrokiem;
sem_t* egzekucja;
sem_t* powyroku;
sem_t* miejsce;
sem_t* kolejka;

void term(int s)
{
	for(int i=0; i<50; i++)
    {
    	sem_destroy(&(u->klienci[i]));
    }
    munmap(u,sizeof(struct ultimate));
    shm_unlink("commonwealth");
    
    sem_close(ifsleep);
    sem_close(klient);
    sem_close(przedwyrokiem);
    sem_close(egzekucja);
    sem_close(powyroku);
    sem_close(miejsce);
    sem_close(kolejka);
    sem_unlink("/ifsleep");
    sem_unlink("/klient");
    sem_unlink("/przedwyrokiem");
    sem_unlink("/egzekucja");
    sem_unlink("/powyroku");
    sem_unlink("/miejsce");
    sem_unlink("/kolejka");

	exit(EXIT_SUCCESS);
}

struct buf next()
{
    if (u->top_fifo==0) u->top_fifo=49;
    else u->top_fifo--;
    struct buf b=u->fifo[u->top_fifo];
    return b;
}

void golibroda_action(int n)
{
    struct buf b;
    sem_wait(ifsleep);
    if (u->ilefree==n)
    {
        printf("%s\n","Golibroda mdleje z wrażenia");
        u->sleepp=1;
        sem_post(ifsleep);
        sem_wait(klient);
        printf("%s\n","Golibroda odzyskuje przytomność");
    }
    else
    {
        b=next();
        u->strz=b.pid;
        printf("%d\n",b.pid);
        sem_post(&(u->klienci[b.sem]));
        sem_post(ifsleep);
    }
    sem_post(przedwyrokiem);
    sem_wait(egzekucja);
    printf("Golibroda zaczyna ścięcie skazańca nr %d\n",u->strz);
    printf("Golibroda ściął skazańca nr %d\n",u->strz);
    sem_post(powyroku);
    sem_wait(egzekucja);
}

int main(int argc,char const *argv[])
{
    if (argc<2)
    {
    	printf("%s\n","Za mało argumentów");
    	return 1;
    }
    int n=atoi(argv[1]);
    signal(SIGINT,term);

    int c=shm_open("commonwealth",O_RDWR | O_CREAT,0666);
    ftruncate(c,sizeof(struct ultimate));
    u=(struct ultimate*)mmap(NULL,sizeof(struct ultimate),0666,MAP_SHARED,c,0);
    close(c);
    u->sleepp=0;
    u->top_klienci=0;
    u->top_fifo=0;
    for(int i=0; i<50; i++) sem_init(&(u->klienci[i]),1,0);

    ifsleep=sem_open("/ifsleep",O_CREAT,0777,1);
    klient=sem_open("/klient",O_CREAT,0777,0);
    przedwyrokiem=sem_open("/przedwyrokiem",O_CREAT,0777,0);
    egzekucja=sem_open("/egzekucja",O_CREAT,0777,0);
    powyroku=sem_open("/powyroku",O_CREAT,0777,0);
    miejsce=sem_open("/miejsce",O_CREAT,0777,1);
    kolejka=sem_open("/kolejka",O_CREAT,0777,1);

    u->ilefree=n;
    while (1)
    {
    	golibroda_action(n);
    }
}