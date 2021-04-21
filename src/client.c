#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h> 
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#include "delay.h"
#include "client.h"
#include "thread.h"

pthread_t threads[10000]; 
pid_t pid;
pthread_t daddy_thread;
int fdServerFifo;
char* publicFifo;
int sizeOfThreads = 0;

//teste
void* func(void* arg){
    pthread_detach(pthread_self());
    
    struct Arg_Thread *thread = (struct Arg_Thread*) arg;
    printf("%ld -- %d -- %d\n",pthread_self(),thread->request_id, thread->task );
    pthread_exit(NULL);
}


void signalAlarmHandler(int signo){   //faltam tweaks
    if( pthread_self()!=daddy_thread)
    {
        //escrever no log que não tem tempo de registo porque o cliente expirou
        printf("O cliente expirou");
        pthread_cancel(pthread_self());
    }
    else{
        for(int i = 0; i<sizeOfThreads; i++)
            pthread_kill(threads[i],SIGALRM); 
        sleep(1);
        unlink(publicFifo);
        printf("Cliente finalizado com %d threads\n", sizeOfThreads);
        exit(0);
    }
}


int main(int argc, char* argv[]){
    struct Arg_Thread args;
    daddy_thread = pthread_self();
    int request_id = 0;
    int n_random;
    srand(time(NULL));

 
    signal(SIGALRM, signalAlarmHandler);
    publicFifo = argv[2];

    if(argc!= 3 ){
        fprintf(stderr, "Invalid number of arguments : the command should be something like ./client <n_sec> <fifoname>\n");
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[1])<=0){
        fprintf(stderr, "The n_secs should be a valid number greater than 0\n");
        exit(EXIT_FAILURE);
    }

    pid = getpid();

    //abrir fifo público
    if(mkfifo(publicFifo, FIFO_MODE) < 0) {
        if(errno == EEXIST) 
            fprintf(stderr, "FIFO '%s' already exists\n", argv[2]);
        else {
            fprintf(stderr, "Can't create server FIFO!\n");
            exit(EXIT_FAILURE);
        }
    }
    if((fdServerFifo = open(argv[2], O_WRONLY))<0)
    {
        fprintf(stderr,"Error in opening public FIFO\n");
    }
    
    alarm(atoi(argv[1]));
    //Loop de criação de threads
    while(true){
        n_random = rand()%9 +1; //Numero de 1 a 9
        args.request_id = request_id++;
        args.task = n_random;
        //criação de thread         
        pthread_create(&threads[sizeOfThreads],NULL,threadFunction,&args);
        //pthread_join(threads[sizeOfThreads], NULL);
        sizeOfThreads++;
        
        int time = rand()%501+500;
        usleep(time); //pausa entre cada cliente
    }
}



