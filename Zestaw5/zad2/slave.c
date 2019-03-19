#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main(int argc,char const *argv[])
{
	printf("Slave number %i %s\n",getpid(),"has been assigned to some useless work");
	srand(time(NULL));
	char *name=(char*)argv[1];
	int n=atoi(argv[2]);
	if (n<=0)
	{
		printf("%s\n","Wrong number of useless work cycles");
		return -1;
	}
	char *buf=(char*)calloc(1,sizeof(char*));

	int f=open(name,O_WRONLY);
	printf("My Slave Serial Number is: %d\n",getpid());

	for (int i=0; i<n; i++)
	{
		char date[50];
		FILE* p=popen("date","r");
		fgets(date,50,p);
		pclose(p);

		char line[512];
		sprintf(line,"%i %s",getpid(),date);
		strcpy(date,"");

		lseek(f,0,SEEK_SET);
		write(f,line,strlen(line));
		sleep(rand()%4+2);
	}
	printf("Slave number %i %s\n",getpid(),"has died due to starvation and working conditions breaking the basic human laws");
    close(f);
	return 0;
}