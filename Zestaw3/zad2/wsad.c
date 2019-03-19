#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

void execute(char *name)
{
	FILE *f=fopen(name,"rt");
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
			fclose(f);
			execvp(x[0],x);
			exit(EXIT_FAILURE);
		}
		int st;
		waitpid(child,&st,0);
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
	}
}

int main(int argc,char const *argv[])
{
	char *name=(char*)argv[1];
	execute(name);
	exit(EXIT_SUCCESS);
}