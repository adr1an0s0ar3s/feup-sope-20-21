#include "serverMain.h"
#include "serverConsumer.h"
#include "serverProducer.h"
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
int isServerClosed = false;
int nsecs;
int bufsize = 20;
char* filename;

int main(int argc, char* argv[]) {

    if (verifyInput(argc, argv) != 0) exit(EXIT_FAILURE);
    printf("!\n");
    daddy_thread = pthread_self();
    buffer = createQueue(bufsize);
    filename = argv[argc-1];

    
    printf("!\n");
    // Create consumer thread
    pthread_t thread;
    pthread_create(&thread, NULL, thread_consumer, NULL);
    
    signal(SIGALRM, signalAlarmHandler);
    alarm(nsecs);
    
    if (openPublicFIFO(argv[argc-1]) != 0) exit(EXIT_FAILURE);

    Message message;

    // Loop with ...
    int readStatus;
    printf("Quase loop\n");
    while (!isServerClosed) {

        // Read Message from publicFifo
        if ((readStatus = read(publicFifoFD, &message, sizeof(Message))) == -1) {   // No message to be read
                fprintf(stderr, "Error when trying to read a Message from public FIFO\n");
                exit(EXIT_FAILURE);
            //}
        }
        else if (readStatus == 0) continue;

        // Logging the reception of the message
        write_operation(message, RECVD);

        // Creation of the producer thread
        Message * messageCopy = (Message *) malloc(sizeof(Message));
        memcpy(messageCopy, &message, sizeof(Message));
        pthread_create(&thread, NULL, thread_producer, (void *) messageCopy);
    }
 
    // Waiting for threads to finish
    
    
}

int verifyInput(int argc, char* argv[]) {

    if (argc < 4 || argc > 6) {
        fprintf(stderr, "Invalid number of arguments : the command should be something like %s <-t n_sec> [-l bufsz] <fifoname>\n", argv[0]);
        return 1;
    }

    bool validFormat = false;

    for (int i = 1; i < argc - 1; i += 2) {
        if (strcmp(argv[i], "-t") == 0) {
            nsecs = atoi(argv[i + 1]);
            if (nsecs <= 0) {
                fprintf(stderr, "The n_secs should be a valid number greater than 0\n");
                return 1;
            }
            validFormat = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            bufsize = atoi(argv[i + 1]);
            if (bufsize <= 0) {
                fprintf(stderr, "The bufsize should be a valid number greater than 0\n");
                return 1;
            }
        }
    }

    if (!validFormat) {
        fprintf(stderr, "The flag -t is required\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

int openPublicFIFO(char filename[]) {

    if (mkfifo(filename, 0777) < 0) {
        if (errno == EEXIST) fprintf(stderr, "FIFO '%s' already exists\n", filename);
        else fprintf(stderr, "Can't create server FIFO!\n");
    }

    if ((publicFifoFD = open(filename, O_RDONLY)) == -1) {
        fprintf(stderr, "Error in opening public FIFO\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;

}

void signalAlarmHandler(int signo) {
    if (pthread_self() == daddy_thread) isServerClosed = true;

    close(publicFifoFD);
    unlink(filename);
    
    sleep(3);
    pthread_mutex_destroy(&mutex);
    printf("Size of buffer final: %d\n", buffer->size);
    freeQueue(buffer); 

    exit(EXIT_SUCCESS);   
}
