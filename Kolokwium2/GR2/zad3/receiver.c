#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define KEY  "./queuekey"

struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               int mtext;    /* message data */
           };

int main() {
        sleep(1);
        int val = 0;


	/**********************************
	Otworz kolejke systemu V "reprezentowana" przez KEY
	**********************************/
    int q=msgget(ftok(KEY,0),0);
	


	while(1)
 	{	
	    /**********************************
	    Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
	    obowiazuja funkcje systemu V
	    ************************************/

 			struct msgbuf buf;

 			struct msqid_ds *buf2;
 			buf2=(struct msqid_ds *)malloc(sizeof(struct msqid_ds));
 			msgctl(q,IPC_STAT,buf2);
 			if (buf2->msg_qnum==0)
 			{
 				perror("Koniec wiadomości");
 				return 0;
 			}



	    	msgrcv(q,&buf,sizeof(int),0,0);
	    	val=buf.mtext;

        	 printf("%d square is: %d\n", val, val*val);
 
	}




	/*******************************
	posprzataj
	********************************/
	msgctl(q,IPC_RMID,NULL);

     return 0;
   }
