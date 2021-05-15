#include "serverMain.h"

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

pthread_t daddy_thread;
Queue buffer;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int publicFifoFD;
int isClientClosed = 0, isServerClosed = 0;
unsigned int seed;

int main(int argc, char* argv[]) {

    if (verifyInput(argc, argv) != 0) exit(EXIT_FAILURE);

    pthread_t thread;
    seed = time(NULL);
    daddy_thread = pthread_self();

    &buffer = createQueue(20);  // TODO: ler capacidade
    
    signal(SIGALRM, signalAlarmHandler);
    alarm(atoi(argv[2]));

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // Loop with ...
    while (!isClientClosed && !isServerClosed) {
        if ((read = getline(&line, &len, publicFifoFD)) != -1) < 0) {
            printf("Nothing\n");
    }
        // Creation of the thread
        //pthread_create(&thread, NULL, threadFunction, NULL);
        break;  // Temporary
    }

    // Waiting for threads to finish
    sleep(1);

    // FOR TESTING PURPOSES
    printf("Cliente finalizado com %d threads\n", sizeOfThreads);

    // TODO: LIBERTAR BUFFER
    close(publicFifoFD);
    if (line) free(line);
   
    exit(EXIT_SUCCESS);
}

int verifyInput(int argc, char* argv[]) {
    if (argc < 4 || argc > 6) {
        fprintf(stderr, "Invalid number of arguments : the command should be something like %s <-t n_sec> [-l bufsz] <fifoname>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-t")) {
        fprintf(stderr, "The flag -t is required\n");
        return 1;
    }

    if (atoi(argv[2]) <= 0) {
        fprintf(stderr, "The n_secs should be a valid number greater than 0\n");
        return 1;
    }

    // TODO: A ordem das flags n devia importar
    return 0;
}

int openPublicFIFO(char filename[]) {
    if (makefifo(filename, FIFO_MODE) < 0) {
        if (errno == EEXIST) {
            fprintf(stderr, "FIFO '%s' already exists\n", filename);
        } else {
            fprintf(stderr, "Can't create server FIFO!\n");
        }
        return 1;
    }

    if ((publicFifoFD = open(filename, O_WRONLY)) < 0) {
        fprintf(stderr,"Error in opening public FIFO\n");
        return 1;
    }

    return 0;
}

void signalAlarmHandler(int signo) {
    if (pthread_self() == daddy_thread) isServerClosed = true;
}
