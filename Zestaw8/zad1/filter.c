#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int w;
int ww;
int h;
int maks;
int c;
int **wyn;
int **t;
double **fi;

int** wczytaj_obraz(char *name)
{
	FILE *f=fopen(name,"r");
	if (f==NULL)
	{
		printf("%s\n","Zła nazwa obrazu");
		exit(EXIT_FAILURE);
	}
	char line[100];
	int lnr=0;
	while (fgets(line,100,f)!=NULL)
	{
		lnr+=1;
		if (lnr==2)
		{
			char wh[2][3];
			int i=0;
			while (line[i]!=' ')
			{
				sprintf(wh[0]+i,"%c",(char)line[i]);
				i++;
			}

			int ile=0;
			while (line[i]!='\n')
			{
				sprintf(wh[1]+ile,"%c",(char)line[i]);
				i++;
				ile++;
			}
			w=atoi(wh[0]);
			h=atoi(wh[1]);
		}

		if (lnr==3)
		{
			int i=0;
			char b[3];
			while (line[i]!='\n')
			{
				b[i]=line[i];
				i++;
			}
			maks=atoi(b);
			break;
		}
	}

	int **t=(int**)malloc(h*sizeof(int*));
	for (int g=0; g<h; g++)
	{
		t[g]=(int*)malloc(w*sizeof(int));
	}
	int ileh=0;
	int ilew=0;

	while (fgets(line,100,f)!=NULL)
	{
		int i=0;
		while (line[i]!='\n' && line[i]!='\r')
		{
			char *buf=(char*)malloc(3*sizeof(char));
			int ile=0;
			while (line[i]==' ') i++;
			while (line[i]!=' ' && line[i]!='\n' && line[i]!='\r')
			{
				buf[ile]=line[i];
				i++;
				ile++;
			}
			ile=0;

			if (line[i]==' ' || line[i]=='\n' || line[i]=='\r')
			{
				t[ileh][ilew]=atoi(buf);
				ilew++;
				while (line[i]==' ') i++;
			}

			if (ilew==w)
			{
				ilew=0;
				ileh++;
			}
			if (ileh==h) break;
		}
		if (ileh==h) break;
	}
	fclose(f);
	return t;
}

double **wczytaj_filtr(char *name)
{
	FILE *f=fopen(name,"r");
	if (f==NULL)
	{
		printf("%s\n","Zła nazwa filtru");
		exit(EXIT_FAILURE);
	}
	char line[100];
	if (fgets(line,100,f)!=NULL) c=atoi(line);

	double **t=(double**)malloc(c*sizeof(double*));
	for (int g=0; g<c; g++)
	{
		t[g]=(double*)malloc(c*sizeof(double));
	}

	int ile=0;
	int h2=0;
	while (fgets(line,100,f)!=NULL)
	{
		ile++;
		t[h2][ile-1]=atof(line);
		if (ile==c)
		{
			ile=0;
			h2++;
		}
	}
	fclose(f);
	return t;
}

int max(int a,int b)
{
	if (a<b) return b;
	else return a;
}

int min(int a,int b)
{
	if (a<b) return a;
	else return b;
}

void *filtruj(void *nr)
{
	int d=ceil((double)w/(double)ww);
	int s=d*(*(int*)nr);
	for (int i=0; i<h; i++)
	{
		for (int j=s; j<s+d; j++)
		{
			double buf=0.0;
			for (int p=0; p<c; p++)
			{
				for (int q=0; q<c; q++)
				{
					int a=min(max(1,i+1-(int)ceil((double)c/2.0)+p+1),h)-1;
					int b=min(max(1,j+1-(int)ceil((double)c/2.0)+q+1),w)-1;
					double c2=fi[p][q];
					buf+=(double)t[a][b]*c2;
				}
			}
			wyn[i][j]=round(buf);
		}
	}
	pthread_exit(0);
}

void zapisz(char *name3)
{
	FILE *f=fopen(name3,"w");
	fputs("P2\n",f);
	char line[10];
	sprintf(line,"%d %d\n",w,h);
	fputs(line,f);
	sprintf(line,"%d\n",maks);
	fputs(line,f);

	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			sprintf(line,"%d ",wyn[i][j]);
			fputs(line,f);
		}
		fputs("\n",f);
	}
	fclose(f);
}

int main(int argc,char const *argv[])
{
	if (argc<5)
	{
		printf("%s\n","Za mało argumentów");
		exit(EXIT_FAILURE);
	}
	ww=atoi(argv[1]);
	if (ww<=0)
	{
		printf("%s\n","Zła liczba wątków");
		exit(EXIT_FAILURE);
	}
	char *name=(char*)argv[2];
	char *name2=(char*)argv[3];
	char *name3=(char*)argv[4];
	t=wczytaj_obraz(name);
	fi=wczytaj_filtr(name2);

	struct timespec stime;
	struct timespec etime;

	wyn=(int**)malloc(h*sizeof(int*));
	for (int g=0; g<h; g++)
	{
		wyn[g]=(int*)malloc(w*sizeof(int));
	}

	int *ii;
	clock_gettime(CLOCK_REALTIME,&stime);
	pthread_t *tt=(pthread_t*)malloc(ww*sizeof(pthread_t));
	for (int i=0; i<ww; i++)
	{
		ii=(int*)malloc(sizeof(int));
		*ii=i;
		if ((pthread_create(&tt[i],NULL,&filtruj,(void*)ii))!=0)
		{
			perror("Stwórz wątek");
			exit(EXIT_FAILURE);
		}
	}

	for (int i=0; i<ww; i++)
	{
		if ((pthread_join(tt[i],NULL))!=0)
		{
			perror("Joinowanie wątku");
			exit(EXIT_FAILURE);
		}
	}
	clock_gettime(CLOCK_REALTIME,&etime);
	long int s=etime.tv_sec-stime.tv_sec;
	long int ns=etime.tv_nsec-stime.tv_nsec;
	if (ns<0)
	{
		ns+=1000000000;
		s--;
	}
	printf("Czas filtrowania wynosi %lds %ldns, %d wątków\n",s,ns,ww);
	zapisz(name3);
	return 0;
}