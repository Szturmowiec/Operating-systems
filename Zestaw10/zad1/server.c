#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/epoll.h>
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

struct client{
    char *name;
    int f;
    int ping;
};

struct clients{
    int ile;
    struct client *t[10];
};

struct pac{
    char type;
    int d;
    int id;
};

char path[100];
int tcp;
int inet;
int unixx;
struct clients *clients;
int ile=0;
pthread_t t[3];

void pingok(int f)
{
    for (int i=0; i<10; i++)
    {
    	if (clients->t[i]!=NULL && clients->t[i]->f==f) clients->t[i]->ping=1;
    }
}

int client_exist(char *name2)
{
    for (int i=0; i<10; i++)
    {
    	if (clients->t[i]!=NULL && strcmp(clients->t[i]->name,name2)==0) return 1;
    }
    return 0;
}

void delete_client(int f)
{
    for (int i=0; i<10; i++)
    {
    	if (clients->t[i]!=NULL && clients->t[i]->f==f) clients->t[i]=NULL;
    }
    clients->ile--;
}

void add_client(int f,char *name)
{
    for (int i=0; i<10; i++)
    {
    	if (clients->t[i]==NULL)
    	{
    		clients->t[i]=(struct client *)malloc(sizeof(struct client));
    		clients->t[i]->f=f;
    		clients->t[i]->name=name;
    		clients->t[i]->ping=1;
    		clients->ile++;
    		break;
    	}
    }
}

int sendd(int client,char type,char *msg,int ilee)
{
    void *buf;
    struct pac p;
    p.type=type;
    p.d=strlen(msg);
    p.id=ilee;

    buf=malloc(sizeof(struct pac)+p.d);
    memcpy(buf,&p,sizeof(struct pac));
    memcpy(buf+sizeof(struct pac),msg,p.d);
    write(client,buf,sizeof(struct pac)+p.d);
    return 0;
}

int newclient(int data)
{
    struct pac p;
    char buf[100];

    int client=accept(data,NULL,NULL);
    read(client,&p,sizeof(struct pac));
    read(client,buf,p.d);

    if (clients->ile==10)
    {
        sendd(client,'O',"Brak miejsca",0);
        shutdown(client,SHUT_RDWR);
    	close(client);
        return 0;
    }

    int z=client_exist(buf);
    if (z==1)
    {
        sendd(client,'O',"Nazwa zajęta",0);
        shutdown(client,SHUT_RDWR);
    	close(client);
        return 0;
    }
    add_client(client,buf);

    sendd(client,'A',"",0);
    return client;
}

void message(int client)
{
    struct pac p;
    char buf[100];
    read(client,&p,sizeof(struct pac));
    read(client,buf,p.d);
    switch(p.type)
    {
        case 'P':
            pingok(client);
            break;
        case '3':
            printf("id %i: %s\n",p.id,buf);
            break;
        case 'D':
            shutdown(client,SHUT_RDWR);
    		close(client);
            delete_client(client);
            break;
    }
}

void* network(void *arg)
{
    struct epoll_event e;
    int epoll=epoll_create(1);
    e.events=EPOLLIN;
    e.data.fd=inet;
    epoll_ctl(epoll,EPOLL_CTL_ADD,inet,&e);
    e.data.fd=unixx;
    epoll_ctl(epoll,EPOLL_CTL_ADD,unixx,&e);

    struct epoll_event events[10];
    int fds;
    int c;
    while(1)
    {
        fds=epoll_wait(epoll,events,10,-1);
        for (int i=0; i<fds; i++)
        {
            if (events[i].data.fd==inet || events[i].data.fd==unixx)
            {
                c=newclient(events[i].data.fd);
                if (c!=0)
                {
                    e.data.fd=c;
                    epoll_ctl(epoll,EPOLL_CTL_ADD,c,&e);
                }
            }
            else message(events[i].data.fd);
        }
    }
}

void* terminal(void *arg)
{
    char* line;
    size_t n=0;
    while(1)
    {
        getline(&line,&n,stdin);
    	if (clients->ile>0)
    	{
    		int i=rand()%clients->ile;
    		while (clients->t[i]==NULL && i<10) i++;
    		if (i==10)
    		{
    			i=0;
    			while (clients->t[i]==NULL && i<10) i++;
    		}
    		int f=clients->t[i]->f;
        	sendd(f,'C',line,ile++);
        }
        else printf("%s\n","Nie ma klientów");
        n=0;
    }
}

void* ping(void *arg)
{
    while(1)
    {
        for (int i=0; i<10; i++)
        {
            if (clients->t[i]!=NULL && clients->t[i]->ping==0)
            {
                shutdown(clients->t[i]->f,SHUT_RDWR);
    			close(clients->t[i]->f);
                delete_client(clients->t[i]->f);
            }
        }

    	for (int i=0; i<10; i++)
    	{
    		if (clients->t[i]!=NULL) clients->t[i]->ping=0;
    	}

    	for (int i=0; i<10; i++)
    	{
    		if (clients->t[i]!=NULL) sendd(clients->t[i]->f,'p',"ping",0);
    	}
        sleep(1);
    }
}

void term()
{
	for (int i=0; i<3; i++)
    {
    	pthread_cancel(t[i]);
    }
    shutdown(inet,SHUT_RDWR);
    close(inet);
    shutdown(unixx,SHUT_RDWR);
    close(unixx);
    for (int i=0; i<10; i++)
    {
    	if (clients->t[i]!=NULL)
    	{
    		shutdown(clients->t[i]->f,SHUT_RDWR);
    		close(clients->t[i]->f);
    	}
    }
    unlink(path);
}

void sigint()
{
	exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
	clients=(struct clients *)malloc(sizeof(struct clients));
	for (int i=0; i<10; i++)
	{
		clients->t[i]=NULL;
	}

    tcp=atoi(argv[1]);
    strcpy(path,argv[2]);
    struct sockaddr_in inett;
    struct sockaddr_un unixxx;

    inet=socket(AF_INET,SOCK_STREAM,0);
    unixx=socket(AF_UNIX,SOCK_STREAM,0);

    inett.sin_family=AF_INET;
    inett.sin_port=htons(tcp);
    inett.sin_addr.s_addr=htons(0);
    unixxx.sun_family=AF_UNIX;
    strcpy(unixxx.sun_path,path);

    bind(inet,(const struct sockaddr *)&inett,sizeof(struct sockaddr_in));
    listen(inet,10);
    bind(unixx,(const struct sockaddr *)&unixxx,sizeof(struct sockaddr_un));
    listen(unixx,10);

    pthread_create(&t[1],NULL,&network,NULL);
    pthread_create(&t[0],NULL,&terminal,NULL);
    pthread_create(&t[2],NULL,&ping,NULL);

    signal(SIGINT,sigint);
    atexit(term);
    pause();
}