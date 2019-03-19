#include <stdio.h>
#include <pthread.h>

void wait_zero(pthread_mutex_t *mutex, pthread_cond_t *restrict cond, int* var)
{
	pthread_mutex_lock(mutex);
	while (*var==0)
	{
		pthread_cond_wait(cond,mutex);
	}
	pthread_mutex_unlock(mutex);
}

void zm(pthread_mutex_t *mutex, pthread_cond_t *restrict cond, int* var)
{
	pthread_mutex_lock(mutex);
	*var=1;
	pthread_cond_broadcast(cond);
	pthread_mutex_unlock(mutex);
}

int main(int argc,char const *argv[])
{
	pthread_mutex_t x=PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t c=PTHREAD_COND_INITIALIZER;
	int n=7;
	wait_zero(&x,&c,&n);
	zm(&x,&c,&n);
	return 0;
}