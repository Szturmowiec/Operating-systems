#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char const *argv[])
{
	printf("%s\n","The beginning of it all");
	char *name=(char*)argv[1];
	char *buf=(char*)calloc(1,sizeof(char*));
	mkfifo(name,0666);

	int f=open(name,O_RDONLY);
	while(read(f,buf,4096)>0)
	{
	    printf("Yet another line of slavery: %s",buf);
	    strcpy(buf,"");
	}
    close(f);
    printf("%s\n","The end of it all");
	return 0;
}