#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/mman.h>

#define SHM_NAME "/kol_shm"
#define MAX_SIZE 1024

int main(int argc, char **argv)
{
    
   if(argc !=2){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }

   int a=atoi(argv[1]);

   int c=shm_open(SHM_NAME,O_RDWR | O_CREAT,0666);
   ftruncate(c,sizeof(a));
   int *x=mmap(NULL,sizeof(a),PROT_WRITE,MAP_SHARED,c,0);

   *x=a;
   /***************************************
   Utworz/otworz posixowy obszar pamieci wspolnej "reprezentowany" przez SHM_NAME
   zapisz tam wartosc przekazana jako parametr wywolania programu
   posprzataj
   *****************************************/

   sleep(10);
 
   munmap(x,sizeof(a));
   shm_unlink(SHM_NAME);

    return 0;
}
