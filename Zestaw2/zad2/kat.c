#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int compare_time(time_t a,time_t b)
{
	int ra,ma,da;
	int rb,mb,db;
	struct tm* date;
	date=localtime(&a);
	ra=date->tm_year;
	ma=date->tm_mon;
	da=date->tm_mday;
	date=localtime(&b);
	rb=date->tm_year;
	mb=date->tm_mon;
	db=date->tm_mday;

	if (ra>rb) return 1;
	else if (ra<rb) return -1;
		else if (ma>mb) return 1;
			else if (ma<mb) return -1;
				else if (da>db) return 1;
					else if (da<db) return -1;
						else return 0;
}

void print_priv(const struct stat *s)
{
	if (s->st_mode & S_IRUSR) printf("r"); else printf("-");
	if (s->st_mode & S_IWUSR) printf("w"); else printf("-");
	if (s->st_mode & S_IXUSR) printf("x"); else printf("-");
	if (s->st_mode & S_IRGRP) printf("r"); else printf("-");
	if (s->st_mode & S_IWGRP) printf("w"); else printf("-");
	if (s->st_mode & S_IXGRP) printf("x"); else printf("-");
	if (s->st_mode & S_IROTH) printf("r"); else printf("-");
	if (s->st_mode & S_IWOTH) printf("w"); else printf("-");
	if (s->st_mode & S_IXOTH) printf("x"); else printf("-");
}

void search(char *path,DIR *d,struct dirent *e,int cmp,time_t date)
{
	e=readdir(d);
	while(e!=NULL)
	{
		if (e->d_type==DT_DIR)
		{
			if (strcmp(e->d_name,".")==0 || strcmp(e->d_name,"..")==0) continue;
			else
			{

			}
		}
		else
		{
			struct stat *s;
			stat(e->d_name,s);
			if ((cmp==1 && compare_time(s->st_mtime,date)==-1) || (cmp==2 && compare_time(s->st_mtime,date)==1) || (cmp==0 && compare_time(s->st_mtime,date)==0))
			{
				printf("%s","Rozmiar pliku: ");
				printf("%ld\t",s->st_size);
				printf("%s","Uprawnienia: ");
				print_priv(s);
				struct tm *mod=s->st_mtime;
				char b[26];
				printf("%s","Data ostatniej modyfikacji: ");
				strftime(b,26,"%Y-%m-%d %H:%M:%S",mod);
			}
		}
	}
}

int main(int argc,char const *argv[])
{
	DIR* d=opendir(argv[1]);
	char *path=(char*)argv[1];
	struct dirent *e;
	int cmp;
	if (strcmp(argv[2],"<")==0) cmp=1;
	if (strcmp(argv[2],">")==0) cmp=2;
	if (strcmp(argv[2],"=")==0) cmp=0;
	time_t time;
	search(path,d,e,cmp,time);
	return 0;
}