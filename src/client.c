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

pthread_t threads[10000]; 
pid_t pid;
int fdServerFifo;
char* publicFifo;
int sizeOfThreads = 0;

//teste
void* func(void* arg){
    pthread_detach(pthread_self());
    
    struct Arg_Thread *thread = (struct Arg_Thread*) arg;
    printf("%ld -- %d -- %d\n",pthread_self(),thread->request_id, thread->task );
    usleep(4000);
    pthread_exit(NULL);
}


void signalAlarmHandler(int signo){
    for(int i = 0; i<sizeOfThreads; i++){
        pthread_cancel(threads[i]);
    }
    unlink(publicFifo);

    exit(0);

}

int main(int argc, char* argv[]){
    struct Arg_Thread args;
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
    if(mkfifo(publicFifo, 0660) < 0) {
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
        n_random = rand()%8 +1; //Numero de 1 a 9
        args.request_id = request_id++;
        args.task = n_random;
        //criação de thread         
        pthread_create(&threads[sizeOfThreads],NULL,func,&args);
        sizeOfThreads++;
        
       usleep(2000); //pausa entre cada cliente
    }
}



