#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>

//message types:
//A - server accepts client
//O - server rejects client
//R - client confirms register
//D - client sends info about unregistering
//p - server sends ping to client
//P - client responds to ping confirming vital signs
//C - calc request from server
//3 - client sends answer with calculated output - znak 3 bo odp na 3.0 daje zaliczenie

struct pac{
    char type;
    int d;
    int id;
};

char name[100];
int type;
struct sockaddr* adr=NULL;
int f;

int calc(char* dzi)
{
    char opr, tmp[10];
    int i=0;
    while (dzi[i]==' ') i++;
    char *arg1=(char *)malloc(10*sizeof(char));
    char *arg2=(char *)malloc(10*sizeof(char));
    char o;
    int k=0;
    while (dzi[i]!='+' && dzi[i]!='-' && dzi[i]!='*' && dzi[i]!='/')
    {
        arg1[k]=dzi[i];
        i++;
        k++;
    }
    k=0;
    o=dzi[i];
    i++;
    for (i; i<strlen(dzi); i++)
    {
        arg2[k]=dzi[i];
        i++;
        k++;
    }
    int a=atoi(arg1);
    int b=atoi(arg2);
    if (o=='+') return a+b;
    if (o=='-') return a-b;
    if (o=='*') return a*b;
    if (o=='/') return a/b;
}

int sendd(char type,char* msg,int ilee)
{
    struct pac p;
    p.type=type;
    p.d=strlen(msg)+1;
    p.id=ilee;

    void *buf=malloc(sizeof(struct pac)+p.d);
    memcpy(buf,&p,sizeof(struct pac));
    memcpy(buf+sizeof(struct pac),msg,p.d);

    write(f,buf,sizeof(struct pac)+p.d);
    return 0;
}

void network()
{
    while(1)
    {
        struct pac p;
        char buf[100];
        read(f,&p,sizeof(struct pac));
        read(f,buf,p.d);

        int w;
        switch(p.type)
        {
            case 'p':
                sendd('P',"Ping",-1);
                break;
            case 'C':
                w=calc(buf);
                sprintf(buf,"%i",w);
                sendd('3',buf,p.id);
                break;
        }
    }
}

void term()
{
    sendd('D',name,-1);
    shutdown(f,SHUT_RDWR);
    close(f);
}

void sigint()
{
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    strcpy(name,argv[1]);
    if (strcmp(argv[2],"INET")==0)
    {
        struct sockaddr_in *a;
        type=0;
        a=(struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        a->sin_family=AF_INET;
        a->sin_port=htons(atoi(argv[4]));
        inet_pton(AF_INET,argv[3],&(a->sin_addr));
        adr=(struct sockaddr*)a;
    }
    if (strcmp(argv[2],"UNIX")==0)
    {
        struct sockaddr_un* a;
        type=1;
        a=(struct sockaddr_un*)malloc(sizeof(struct sockaddr_un));
        a->sun_family=AF_UNIX;
        strcpy(a->sun_path,argv[3]);
        adr=(struct sockaddr*)a;
    }

    if (type==0)
    {
        f=socket(AF_INET,SOCK_STREAM,0);
        connect(f,adr,sizeof(struct sockaddr_in));
    }
    else
    {
        f=socket(AF_UNIX,SOCK_STREAM,0);
        connect(f,adr,sizeof(struct sockaddr_un));
    }
    sendd('R',name,-1);

    char buf[100];
    struct pac p;
    read(f,&p,sizeof(struct pac));
    read(f,buf,p.d);
    if ((p.type=='A' || p.type=='O') && strcmp(buf,"")!=0) printf("%s\n",buf);
    if (p.type=='O') exit(EXIT_FAILURE);

    signal(SIGINT,sigint);
    atexit(term);
    network();
}