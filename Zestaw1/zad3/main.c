#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "tab.h"

char* generate(int size)
{
	char g[]="abcdefghijklmnoprstuwyz";
	char *s=(char*)calloc(size,sizeof(char));
	for (int i=0; i<size; i++)
	{
		s[i]=g[rand()%23];
	}
	return s;
}

void showtime(struct tms *start,struct tms *end,clock_t realstart, clock_t realend)
{
	printf("real time: %ld\n", realend-realstart);
	printf("user time: %ld\n", end->tms_utime-start->tms_utime);
	printf("system time: %ld\n", end->tms_stime-start->tms_stime);
}

int main(int argc,char const *argv[])
{
	struct bloki* tab=create_static();
	srand(time(NULL));

	int n=atoi(argv[1]);
	int size=atoi(argv[2]);
	int dynamic;
	if (strcmp(argv[3],"dynamic")==0) dynamic=1;
	if (strcmp(argv[3],"static")==0) dynamic=0;

	struct tms *start=malloc(sizeof(struct tms));
	struct tms *end=malloc(sizeof(struct tms));
	clock_t realstart;
	clock_t realend;


	struct bloki* t;
	for (int i=4; i<argc; i++)
	{
		if (strcmp(argv[i],"create_table")==0)
		{
			if (dynamic==1)
			{
				realstart=times(start);
				for (int j=0; j<10000; j++) t=create_dynamic(n,size);
				realend=times(end);
				printf("%s\n", "create_table, dynamic");
				showtime(start,end,realstart,realend);
			}
			else
			{
				realstart=times(start);
				for (int j=0; j<1000000; j++) t=create_static();
				realend=times(end);
				printf("%s\n", "create_table, static");
				showtime(start,end,realstart,realend);
			}
		}
		if (strcmp(argv[i],"search_element")==0)
		{
			realstart=times(start);
			char *w=nearest_value(t,n,size,atoi(argv[i+1]));
			realend=times(end);
			printf("%s\n", "search_element");
			showtime(start,end,realstart,realend);
		}
		if (strcmp(argv[i],"remove")==0)
		{
			if (dynamic==1)
			{

				realstart=times(start);
				for (int j=t->ile-1; j>t->ile-1-atoi(argv[i+1]); j--) remove_dynamic(t,j);
				realend=times(end);
				printf("%s", "remove_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, dynamic");
				showtime(start,end,realstart,realend);
			}
			else
			{
				realstart=times(start);
				for (int j=t->ile-1; j>t->ile-1-atoi(argv[i+1]); j--) remove_static(t,j);
				realend=times(end);
				printf("%s", "remove_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, static");
				showtime(start,end,realstart,realend);
			}
		}
		if (strcmp(argv[i],"add")==0)
		{
			if (dynamic==1)
			{
				realstart=times(start);
				for (int j=0; j<atoi(argv[i+1]); j++) add_dynamic(t,generate(size),size);
				realend=times(end);
				printf("%s", "add_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, dynamic");
				showtime(start,end,realstart,realend);
			}
			else
			{
				realstart=times(start);
				for (int j=0; j<atoi(argv[i+1]); j++) add_static(t,generate(size));
				realend=times(end);
				printf("%s", "add_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, static");
				showtime(start,end,realstart,realend);
			}
		}
		if (strcmp(argv[i],"add_and_remove")==0)
		{
			if (dynamic==1)
			{
				realstart=times(start);
				for (int j=0; j<atoi(argv[i+1]); j++)
				{
					add_dynamic(t,generate(size),size);
					remove_dynamic(t,t->ile-1);
				}
				realend=times(end);
				printf("%s", "add_and_remove_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, dynamic");
				showtime(start,end,realstart,realend);
			}
			else
			{
				realstart=times(start);
				for (int j=0; j<atoi(argv[i+1]); j++)
				{
					add_static(t,generate(size));
					remove_static(t,t->ile-1);
				}
				realend=times(end);
				printf("%s", "add_and_remove_");
				printf("%s", argv[i+1]);
				printf("%s\n", "_blocks, static");
				showtime(start,end,realstart,realend);
			}
		}
	}
	printf("\n\n");
	return 0;
}