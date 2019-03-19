#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>

char** get_command(char *s)
{
	int d=strlen(s);
	d--;
	int ile=1;
	for (int i=0; i<d; i++)
	{
		if (s[i]==' ') ile++;
	}
	char **t=(char**)calloc(ile,sizeof(char*));
	int j=0;
	int k=0;
	t[j]=(char*)calloc(50,sizeof(char));
	for (int i=0; i<d; i++)
	{
		if (s[i]==' ')
		{
			j++;
			k=0;
			t[j]=(char*)calloc(50,sizeof(char));
		}
		else
		{
			t[j][k]=s[i];
			k++;
		}
	}
	return t;
}

void show_stats(struct rusage *u1,struct rusage *u2)
{
	printf("\n");
	printf("%s\n","system time:");
	printf("%lds, %ldms\n",u1->ru_stime.tv_sec-u2->ru_stime.tv_sec,u1->ru_stime.tv_usec-u2->ru_stime.tv_usec);
	printf("%s\n","user time:");
	printf("%lds, %ldms\n",u1->ru_utime.tv_sec-u2->ru_utime.tv_sec,u1->ru_utime.tv_usec-u2->ru_utime.tv_usec);
	printf("%s\n","maximum resident set size:");
	printf("%ld KB\n",u1->ru_maxrss);
	printf("\n");
}

void execute(char *name,int cpu,int mem)
{
	FILE *f=fopen(name,"rt");
	struct rlimit *ogr=malloc(sizeof(struct rlimit));
	mem=mem*1024*1024;
	struct rusage *u1=malloc(sizeof(struct rusage));
	struct rusage *u2=malloc(sizeof(struct rusage));

	while(feof(f)==0)
	{
		char *t=(char*)calloc(1,sizeof(char*));
		fgets(t,99,f);
		int d=strlen(t);
		char **x;
		if (d!=0) x=get_command(t);
		else exit(EXIT_FAILURE);

		pid_t child;
		child=fork();
		if (child<0)
		{
			printf("%s","error in: ");
			printf("%s",t);
		}
		if (child==0)
		{
			ogr->rlim_cur=(rlim_t)cpu;
			ogr->rlim_max=(rlim_t)cpu;
			if (setrlimit(RLIMIT_CPU,ogr)<0)
			{
				printf("%s","error in: ");
				printf("%s",t);
				exit(EXIT_FAILURE);
			}

			ogr->rlim_cur=(rlim_t)mem;
			ogr->rlim_max=(rlim_t)mem;
			if (setrlimit(RLIMIT_AS,ogr)<0)
			{
				printf("%s","error in: ");
				printf("%s",t);
				exit(EXIT_FAILURE);
			}

			fclose(f);
			execvp(x[0],x);
			exit(EXIT_FAILURE);
		}

		int st;
		waitpid(child,&st,0);
		getrusage(RUSAGE_CHILDREN,u1);
		if (!WIFEXITED(st))
		{
			printf("%s","error in: ");
			printf("%s",t);
			exit(EXIT_FAILURE);
		}
		if (WEXITSTATUS(st)!=0)
		{
			printf("%s","error in: ");
			printf("%s",t);
		}
		else
		{
			show_stats(u1,u2);
			u2->ru_stime.tv_sec=u1->ru_stime.tv_sec;
			u2->ru_stime.tv_usec= u1->ru_stime.tv_usec;
			u2->ru_utime.tv_sec=u1->ru_utime.tv_sec;
			u2->ru_utime.tv_usec=u1->ru_utime.tv_usec;
			u2->ru_maxrss=u1->ru_maxrss;
		}
	}
}

int main(int argc,char const *argv[])
{
	char *name=(char*)argv[1];
	int cpu=atoi(argv[2]);
	int mem=atoi(argv[3]);
	execute(name,cpu,mem);
	exit(EXIT_SUCCESS);
}