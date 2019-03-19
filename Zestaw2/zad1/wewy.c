#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>

void generate(char *name,int n,int size)
{
	FILE *f=fopen(name,"wb");
	char** t=(char**)calloc(n,sizeof(char*));

	for (int i=0; i<n; i++)
	{
		t[i]=(char*)calloc(size,sizeof(char));
		for (int j=0; j<size; j++)
		{
			char s='a'+rand()%26;
			t[i][j]=s;
		}
		fwrite(t[i],size,sizeof(char),f);
	}
	fclose(f);
}

void copy_sys(char *name1,char *name2,int n,int b)
{
	int p1=open(name1,O_RDONLY);
	int p2=open(name2,O_WRONLY | O_CREAT,0666);

	for (int i=0; i<n; i++)
	{
		char *tab=(char*)calloc(b,sizeof(char));
		read(p1,tab,b);
		write(p2,tab,b);
	}
	close(p1);
	close(p2);
}

void copy_lib(char *name1,char *name2,int n,int b)
{
	FILE *p1=fopen(name1,"rb");
	FILE *p2=fopen(name2,"wb");

	for (int i=0; i<n; i++)
	{
		char *tab=(char*)calloc(b+2,sizeof(char));
		fread(tab,b,sizeof(char),p1);
		fwrite(tab,b,sizeof(char),p2);
	}
	fclose(p1);
	fclose(p2);
}

void sort_sys(char *name,int n,int b)
{
	int f=open(name,O_RDWR);
	char** t=(char**)calloc(n,sizeof(char*));
	for (int i=0; i<n; i++)
	{
		t[i]=(char*)calloc(b,sizeof(char));
		read(f,t[i],b);
	}

	for (int i=1; i<n; i++)
	{
		int j;
		int x=(int)t[i][0];
		char *r=t[i];
		for (j=i-1; j>=0; j--)
		{
			if (x<(int)t[j][0]) t[j+1]=t[j];
			else break;
		}
		t[j+1]=r;
	}

	lseek(f,0,SEEK_SET);
	for (int i=0; i<n; i++)
	{
		write(f,t[i],b);
	}
	close(f);
}

void sort_lib(char *name,int n,int b)
{
	FILE *f=fopen(name,"rb");
	char** t=(char**)calloc(n,sizeof(char*));
	for (int i=0; i<n; i++)
	{
		t[i]=(char*)calloc(b,sizeof(char));
		fread(t[i],b,sizeof(char),f);
	}
	fclose(f);

	f=fopen(name,"wb");
	for (int i=1; i<n; i++)
	{
		int j;
		int x=(int)t[i][0];
		char *r=t[i];
		for (j=i-1; j>=0; j--)
		{
			if (x<(int)t[j][0]) t[j+1]=t[j];
			else break;
		}
		t[j+1]=r;
	}

	for (int i=0; i<n; i++)
	{
		fwrite(t[i],b,sizeof(char),f);
	}
	fclose(f);
}

void showtime(struct tms *start,struct tms *end)
{
	printf("user time: %ld\n", end->tms_utime-start->tms_utime);
	printf("system time: %ld\n", end->tms_stime-start->tms_stime);
}

int main(int argc,char const *argv[])
{
	srand(time(NULL));

	struct tms *start=malloc(sizeof(struct tms));
	struct tms *end=malloc(sizeof(struct tms));
	clock_t realstart;
	clock_t realend;

	if (strcmp(argv[1],"generate")==0)
	{
		char *name=(char*)argv[2];
		generate(name,atoi(argv[3]),atoi(argv[4]));
	}

	if (strcmp(argv[1],"sort")==0)
	{
		if (strcmp(argv[5],"sys")==0)
		{
			char *name=(char*)argv[2];
			realstart=times(start);
			sort_sys(name,atoi(argv[3]),atoi(argv[4]));
			realend=times(end);
			printf("%s","sort_sys, record_size: ");
			printf("%d",atoi(argv[4]));
			printf("%s",", records_quantity: ");
			printf("%d\n",atoi(argv[3]));
			showtime(start,end);
		}
		if (strcmp(argv[5],"lib")==0)
		{
			char *name=(char*)argv[2];
			realstart=times(start);
			sort_lib(name,atoi(argv[3]),atoi(argv[4]));
			realend=times(end);
			printf("%s","sort_lib, record_size: ");
			printf("%d",atoi(argv[4]));
			printf("%s",", records_quantity: ");
			printf("%d\n",atoi(argv[3]));
			showtime(start,end);
		}
	}

	if (strcmp(argv[1],"copy")==0)
	{
		if (strcmp(argv[6],"sys")==0)
		{
			char *name1=(char*)argv[2];
			char *name2=(char*)argv[3];
			realstart=times(start);
			copy_sys(name1,name2,atoi(argv[4]),atoi(argv[5]));
			realend=times(end);
			printf("%s","copy_sys, record_size: ");
			printf("%d",atoi(argv[5]));
			printf("%s",", records_quantity: ");
			printf("%d\n",atoi(argv[4]));
			showtime(start,end);
		}
		if (strcmp(argv[6],"lib")==0)
		{
			char *name1=(char*)argv[2];
			char *name2=(char*)argv[3];
			realstart=times(start);
			copy_lib(name1,name2,atoi(argv[4]),atoi(argv[5]));
			realend=times(end);
			printf("%s","copy_lib, record_size: ");
			printf("%d",atoi(argv[5]));
			printf("%s",", records_quantity: ");
			printf("%d\n",atoi(argv[4]));
			showtime(start,end);
		}
	}

	printf("\n");
	return 0;
}
