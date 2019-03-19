#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               int mtext;    /* message data */
           };



#define KEY "./queuekey"

int main(int argc, char* argv[])
{
 
 if(argc !=2){
   printf("Not a suitable number of program parameters\n");
   return(1);
 }

 key_t k=ftok(KEY,0);

 int x=atoi(argv[1]);

 /******************************************************
 Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
 Wyslij do niej wartosc przekazana jako parametr wywolania programu 
 Obowiazuja funkcje systemu V
 ******************************************************/
 int q=msgget(k,IPC_CREAT | IPC_EXCL | 0666);
 if (errno==EEXIST)
 {
 	q=msgget(k,0);
 }

 struct msgbuf buf;
 buf.mtype=1;
 buf.mtext=x;

 msgsnd(q,&buf,sizeof(int),0);



  return 0;
}



