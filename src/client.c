#include "client.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
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

#include "thread.h"

pthread_t daddy_thread;
int publicFifoFD;
char* publicFifo;
int sizeOfThreads = 0;
int isClientClosed = false, isServerClosed = false;
unsigned int seed;

//Testing
int n_IWANT = 0;
int n_GAVUP = 0;
int n_CLOSD = 0;
int n_GOTRS = 0;

void signalAlarmHandler(int signo) {
    if (pthread_self() == daddy_thread) isClientClosed = true;
}

int main(int argc, char* argv[]) {
    
    // Verifying inputs
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments : the command should be something like ./client <-t n_sec> <fifoname>\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-t")) {
        fprintf(stderr, "The flag -t is required\n");
        exit(EXIT_FAILURE);
    }

    if (atoi(argv[2]) <= 0) {
        fprintf(stderr, "The n_secs should be a valid number greater than 0\n");
        exit(EXIT_FAILURE);
    }

    seed = time(NULL);
    pthread_t thread;
    daddy_thread = pthread_self();
 
    // Changing the alarm handler
    signal(SIGALRM, signalAlarmHandler);

    publicFifo = argv[3];

    // Opening the public FIFO
    if (mkfifo(publicFifo, FIFO_MODE) < 0) {
        if (errno == EEXIST) {
            fprintf(stderr, "FIFO '%s' already exists\n", argv[3]);
        } else {
            fprintf(stderr, "Can't create server FIFO!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    if ((publicFifoFD = open(argv[3], O_WRONLY)) < 0) {
        fprintf(stderr,"Error in opening public FIFO\n");
    }
    
    alarm(atoi(argv[2]));

    // Loop with the creation of pseudo random threads
    while (!isClientClosed && !isServerClosed) {
        
        // Creation of the thread
        sizeOfThreads++;
        pthread_create(&thread, NULL, threadFunction, NULL);

        usleep(rand_r(&seed) % 501 + 500); // Pseudo random pause
    }

    // Waiting for threads to finish
    sleep(1);

    printf("Cliente finalizado com %d threads\n", sizeOfThreads);
    printf("IWANT: %d; GOTRS: %d; GAVEUP: %d; CLOSD: %d\n", n_IWANT, n_GOTRS, n_GAVUP, n_CLOSD);
    exit(EXIT_SUCCESS);
}
