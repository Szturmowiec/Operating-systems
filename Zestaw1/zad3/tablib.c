#include <stdlib.h>
#include <string.h>
#include "tab.h"

struct bloki* create_dynamic(int n,int size)
{
	struct bloki* t=calloc(1,sizeof(struct bloki));
	t->t=(char**)calloc(n,size*sizeof(char*));
	t->ile=0;
	return t;
}

void delete_dynamic(struct bloki* t)
{
	free(t->t);
}

struct bloki* create_static()
{
	char tab[1000][1000];
	char* x=(char*)tab;
	struct bloki* t=calloc(1,sizeof(struct bloki));
	t->t=&x;
	t->ile=0;
	return t;
}

void add_dynamic(struct bloki* t,char* s,int size)
{
	t->t[t->ile]=calloc(size,sizeof(char));
	strcpy(t->t[t->ile],s);
	t->ile++;
}

void add_static(struct bloki* t,char* s)
{
	t->t[t->ile]=s;
	t->ile++;
}

void remove_dynamic(struct bloki* t,int id)
{
	for (int i=id; i<t->ile; i++)
	{
		t->t[i]=t->t[i+1];
	}
	t->ile--;
}

void remove_static(struct bloki* t,int id)
{
	for (int i=id; i<t->ile; i++)
	{
		t->t[i]=t->t[i+1];
	}
	t->ile--;
}

char* nearest_value(struct bloki* t,int n,int size,int x)
{
	int cel=0;
	for (int g=0; g<size; g++)
	{
		cel=cel+(int)t->t[x][g];
	}

	int closest=0;
	int closestid=0;
	for (int i=0; i<n; i++)
	{
		int k=0;
		for (int j=0; j<size; j++)
			{
				k=k+(int)t->t[i][j];
			}
		if ((i==0 || abs(cel-k)<=closest) && i!=x)
		{
			closest=abs(cel-k);
			closestid=i;
		}
	}
	return t->t[closestid];
}