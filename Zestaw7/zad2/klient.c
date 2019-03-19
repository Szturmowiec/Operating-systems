#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

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

void zajmij_miejsce(struct buf b)
{
    u->fifo[u->top_fifo]=b;
    if (u->top_fifo==49) u->top_fifo=0;
    else u->top_fifo++;
}

int klient_sem_id()
{
    if (u->top_klienci==49) u->top_klienci=0;
    else u->top_klienci++;
    return u->top_klienci;
}

void klient_action(int s)
{
    printf("Klient numer %d\n",getpid());
    int sem=klient_sem_id();
    struct buf b;
    b.pid=getpid();
    b.sem=sem;
    while(s>0)
    {
        sem_wait(ifsleep);
        if (u->sleepp==1)
        {
            printf("Klient numer %d",getpid());
            printf("%s\n"," budzi golibrodę");
            u->sleepp=0;
            u->strz=getpid();
            sem_wait(kolejka);
            sem_post(klient);
            sem_post(ifsleep);
        }
        else
        {
            if (u->ilefree>0)
            {
                sem_wait(kolejka);
                printf("Klient numer %d",getpid());
                printf("%s\n"," zajmuje miejsce");
                zajmij_miejsce(b);
                u->ilefree-=1;
                sem_post(kolejka);
                sem_post(ifsleep);
                sem_wait(&(u->klienci[sem]));
                sem_wait(ifsleep);
                sem_wait(kolejka);
                u->ilefree+=1;
                sem_post(ifsleep);
            }
            else
            {
                printf("Klient numer %d",getpid());
                printf("%s\n"," uszedł z życiem");
                sem_post(ifsleep);
            }
        }
        sem_wait(miejsce);
        sem_wait(przedwyrokiem);
        printf("Klient numer %d",getpid());
        printf("%s\n"," Został doprowadzony do gilotyny");
        sem_post(kolejka);
        sem_post(egzekucja);
        sem_wait(powyroku);
        printf("Klient numer %d",getpid());
        printf("%s\n"," Został ścięty");
        sem_post(miejsce);
        sem_post(egzekucja);
        s--;
    }
    printf("Klient numer %d",getpid());
    printf("%s\n"," został wessany przez czarną dziurę");
    exit(EXIT_SUCCESS);
}

int main(int argc,char const *argv[])
{
    if (argc<3) printf("%s\n","Za mało argumentów");
    int ilekli=atoi(argv[1]);
    int s=atoi(argv[2]);

    int c=shm_open("commonwealth",O_RDWR,0);
    u=(struct ultimate*)mmap(NULL,sizeof(struct ultimate),0666,MAP_SHARED,c,0);
    close(c);
    ifsleep=sem_open("/ifsleep",0);
    klient=sem_open("/klient",0);
    przedwyrokiem=sem_open("/przedwyrokiem",0);
    egzekucja=sem_open("/egzekucja",0);
    powyroku=sem_open("/powyroku",0);
    miejsce=sem_open("/miejsce",0);
    kolejka=sem_open("/kolejka",0);

    for (int i=0; i<ilekli; i++)
    {
        pid_t child=fork();
        if (child==0) klient_action(s);
    }
    munmap(u,sizeof(struct ultimate));
    sem_close(ifsleep);
    sem_close(klient);
    sem_close(przedwyrokiem);
    sem_close(egzekucja);
    sem_close(powyroku);
    sem_close(miejsce);
    sem_close(kolejka);
    return 0;
}