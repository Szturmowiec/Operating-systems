#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main ( int argc, char *argv[] )
{
    int i, pid;

    if(argc != 2){
        printf("Not a suitable number of program arguments");
        exit(2);
    }else {
       for (i = 0; i < atoi(argv[1]); i++) {

        pid=fork();
        if (pid==0)
        {
          printf("I'm a child number %d\n",i);
          pid_t p=getpid();
          int p2=(int)p;
          printf("PID %d\n",p2);
          pid_t p3=getppid();
          int p4=(int)p3;
          printf("Parent PID %d\n",p4);
          exit(EXIT_SUCCESS);
        }
        int st;
        waitpid(pid,&st,0);

        //*********************************************************
        //Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1] procesow potomnych, bedacych dziecmi
        //   tego samego procesu macierzystego.
           // Kazdy proces potomny powinien:
               // - "powiedziec ktorym jest dzieckiem",
                //-  jaki ma pid,
                //- kto jest jego rodzicem
           //******************************************************
        }
    }
    return 0;
}
