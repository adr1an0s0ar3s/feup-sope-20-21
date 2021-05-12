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
int publicFifoFD;
int isClientClosed = 0, isServerClosed = 0;
unsigned int seed;

int main(int argc, char* argv[]) {

    if (verifyInput(argc, argv) != 0) exit(EXIT_FAILURE);

    seed = time(NULL);
    daddy_thread = pthread_self();

    pthread_t thread;

    exit(0);
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