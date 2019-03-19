#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem"


// dodane przeze mnie
union semun {
               int              val;    
               struct semid_ds *buf;   
               unsigned short  *array;  
               struct seminfo  *__buf;  
                                           
           };
//koniec dodanego przeze mnie



int main(int argc, char** args){

   if(argc !=4){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }

    /**************************************************
    Stworz semafor systemu V
    Ustaw jego wartosc na 1
    ***************************************************/

   // dodane przeze mnie

   union semun u;
   //u=(union semun *)malloc(sizeof(union semun));
   u.val=1;


   struct sembuf *op;
   op=(struct sembuf *)malloc(sizeof(struct sembuf));

    int sem=semget(ftok(SEM_NAME,0),1,IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
    semctl(sem,0,SETVAL,u);
    //koniec dodanego przeze mnie
    
     
     int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC , 0644);
     
     int parentLoopCounter = atoi(args[1]);
     int childLoopCounter = atoi(args[2]);
        
     char buf[20];
     pid_t childPid;
     int max_sleep_time = atoi(args[3]);
     


     if(childPid=fork()){
      int status = 0;
      srand((unsigned)time(0)); 

        while(parentLoopCounter--){
    	    int s = rand()%max_sleep_time+1;
    	    sleep(s);    
            

            // dodane przeze mnie

            op->sem_op=-1;
            op->sem_num=0;
            op->sem_flg=0;

            semop(sem,op,1);

            //koniec dodanego przeze mnie

	    /*****************************************
	    sekcja krytyczna zabezpiecz dostep semaforem
	    **********************************************/
        
            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter,s);
	    write(fd, buf, strlen(buf));
	    write(1, buf, strlen(buf));

        // dodane przeze mnie
            
        op->sem_op=1;
        op->sem_num=0;
            op->sem_flg=0;

        semop(sem,op,1);

        //koniec dodanego przeze mnie


	    /*********************************
	    Koniec sekcji krytycznej
	    **********************************/

        }
        waitpid(childPid,&status,0);
     }
     else{

	srand((unsigned)time(0)); 
        while(childLoopCounter--){

	    int s = rand()%max_sleep_time+1;
            sleep(s);                
            
            // dodane przeze mnie

            op->sem_op=-1;
            op->sem_num=0;
            op->sem_flg=0;

            semop(sem,op,1);

            //koniec dodanego przeze mnie

	    /*****************************************
	    sekcja krytyczna zabezpiecz dostep semaforem
	    **********************************************/

            
            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter,s);
            write(fd, buf, strlen(buf));
	    write(1, buf, strlen(buf));


        // dodane przeze mnie

        op->sem_op=1;
        op->sem_num=0;
            op->sem_flg=0;

        semop(sem,op,1);

        //koniec dodanego przeze mnie

	    /*********************************
	    Koniec sekcji krytycznej
	    **********************************/

        }
        _exit(0);
     }



     //ta linijka też moja
     semctl(sem,0,IPC_RMID);
     
    /*****************************
    posprzataj semafor
    ******************************/

     close(fd);
    exit(0);
}
     
        
