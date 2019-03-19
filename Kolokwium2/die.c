#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

int reserve_space(key_t key, size_t size, void** ptr)
{
	int c=shmget(key,size,IPC_CREAT | S_IRWXU);
	void *p=*ptr;
	p=shmat(c,NULL,0);
	return c;
}

int main(int argc, char const *argv[])
{
	key_t k=ftok("die.c",0);
	size_t s=7;
	void* ptr;
	reserve_space(k,s,&ptr);
	return 0;
}