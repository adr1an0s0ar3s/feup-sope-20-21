#include "serverMain.h"
#include "serverConsumer.h"
#include "log.h"
#include "queue.h"

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
Queue *buffer;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int publicFifoFD;
int isServerClosed = 0;

int main(int argc, char* argv[]) {

    if (verifyInput(argc, argv) != 0) exit(EXIT_FAILURE);

    daddy_thread = pthread_self();
    buffer = createQueue(20);  // TODO: ler capacidade
    openPublicFIFO(argv[3]);

    // Create consumer thread
    pthread_t thread;
    pthread_create(&thread, NULL, thread_consumer, NULL);
    
    signal(SIGALRM, signalAlarmHandler);
    alarm(atoi(argv[2]));

    Message message;

    // Loop with ...
    while (!isServerClosed) {
        if (read(publicFifoFD, &message, sizeof(Message)) < 0) {
            fprintf(stderr, "Error when trying to read a Message from public FIFO\n");
            exit(EXIT_FAILURE);
        }
        

        //write_operation(message, )
        // Creation of the producer thread
        //pthread_create(&thread, NULL, threadFunction, NULL);
        break;  // Temporary
    }
 
    // Waiting for threads to finish
    sleep(1);

    freeQueue(buffer);
    close(publicFifoFD);
    unlink(argv[3]);
   
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
    if (mkfifo(filename, 0777) < 0) {
        if (errno == EEXIST) fprintf(stderr, "FIFO '%s' already exists\n", filename);
        else fprintf(stderr, "Can't create server FIFO!\n");
        return 1;
    }

    if ((publicFifoFD = open(filename, O_WRONLY)) < 0) {
        fprintf(stderr, "Error in opening public FIFO\n");
        return 1;
    }

    return 0;
}

void signalAlarmHandler(int signo) {
    if (pthread_self() == daddy_thread) isServerClosed = true;
}
