#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct one_command
{
	char **t;
	int pos;
};

struct one_command get_command(char *s,int pos)
{
	int d=strlen(s);
	d--;
	int ile=1;
	for (int i=pos; i<d; i++)
	{
		if (s[i]==' ') ile++;
		if (s[i]=='|') break;
	}
	char **t=(char**)calloc(ile,sizeof(char*));
	int j=0;
	int k=0;
	if (pos<d) t[j]=(char*)calloc(50,sizeof(char));
	for (int i=pos; i<d; i++)
	{
		if (s[i]=='|')
		{
			pos=i+2;
			break;
		}
		if (s[i]==' ')
		{
			j++;
			k=0;
			if (s[i+1]!='|') t[j]=(char*)calloc(50,sizeof(char));
		}
		else if (s[i]!='\n')
		{
			t[j][k]=s[i];
			k++;
			if (i==d-1) pos=d+1;
		}
	}
	struct one_command c;
	c.t=t;
	c.pos=pos;
	return c;
}

int main(int argc,char const *argv[])
{
	FILE *f=fopen("commands","rt");
	while(feof(f)==0)
	{
		char *t=(char*)calloc(1,sizeof(char*));
		fgets(t,99,f);
		int d=strlen(t)-1;

		int ilea=0;
		char ***all=(char***)calloc(50,sizeof(char**));
		char **x;
		int startpos=0;

		while (startpos<d)
		{
			struct one_command c;
			c=get_command(t,startpos);
			startpos=c.pos;
			x=c.t;
			all[ilea]=x;
			ilea++;
		}

		pid_t child[ilea];
		int fd[ilea-1][2];

		for (int i=0; i<ilea; i++)
		{
			pipe(fd[i]);
			child[i]=fork();

			if (child[i]==0)
			{
   				if (i!=0)
   				{
   					dup2(fd[i-1][0],STDIN_FILENO);
   					close(fd[i-1][1]);
   				}
    			if (i!=ilea-1)
    			{
    				dup2(fd[i][1],STDOUT_FILENO);
    				close(fd[i][0]);
    			}
    			execvp(all[i][0],all[i]);
    		}
    		if (i!=0) close(fd[i-1][0]);
			if (i!=ilea-1) close(fd[i][1]);
		}

		int st;
		for (int i=0; i<ilea; i++)
		{
			waitpid(child[i],&st,0);
		}
	}
	return 0;
}