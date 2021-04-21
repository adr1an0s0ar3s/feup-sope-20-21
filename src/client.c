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

pid_t pid;
int fdServerFifo;

int main(int argc, char* argv[]){

    if(argc!= 2 ){
        fprintf(stderr, "Invalid number of arguments : the command should be something like ./client <n_sec> <fifoname>\n");
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[1])<=0){
        fprintf(stderr, "The n_secs should be a valid number greater than 0\n");
        exit(EXIT_FAILURE);
    }

    pid = getpid();

    //abrir fifo público
    if(mkfifo(argv[2], 0660) < 0) {
        if(errno == EEXIST) 
            fprintf(stderr, "FIFO '%s' already exists\n", argv[2]);
        else {
            fprintf(stderr, "Can't create server FIFO!\n");
            exit(EXIT_FAILURE);
        }
    }

    if(fdServerFifo = open(argv[2], O_WRONLY)<0)
    {
        fprintf(stderr,"Error in opening public FIFO\n");
    }

    
}