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

#include <poll.h>

// Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Queue *buffer;
int isServerClosed = false;
int publicFifoFD;
int n_threads = 0;

// Inputs
int nsecs;
int bufsize = 20;
char *filename;

int main(int argc, char* argv[]) {

    if (verifyInput(argc, argv) != 0) exit(EXIT_FAILURE);

    buffer = createQueue(bufsize);

    // Create consumer thread
    pthread_t threads[2000];
    pthread_create(&threads[n_threads++], NULL, thread_consumer, NULL);
    
    signal(SIGALRM, signalAlarmHandler);
    alarm(nsecs);
    
    if (openPublicFIFO(argv[argc-1]) != 0) exit(EXIT_FAILURE);

    Message message;
    int readStatus;

    while (!isServerClosed) {
        // Read Message from publicFifo
        if ((readStatus = read(publicFifoFD, &message, sizeof(Message))) == -1) {
            if (errno == EWOULDBLOCK) continue;
            fprintf(stderr, "Error when trying to read a Message from public FIFO\n");
            exit(EXIT_FAILURE);
        }
        else if (readStatus == 0) break;

        // Logging the reception of the message
        write_operation(message, RECVD);

        // Creation of the producer thread
        Message * messageCopy = (Message *) malloc(sizeof(Message));
        memcpy(messageCopy, &message, sizeof(Message));
        pthread_create(&threads[n_threads++], NULL, thread_producer, (void *) messageCopy);
    }

    unlink(filename);

    // Terminate serverProducer threads
    for (int i = n_threads - 1; i > 0; --i) pthread_join(threads[--n_threads], NULL);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    // Terminate consumerThread
    pthread_join(threads[n_threads-1], NULL);
    n_threads--;

    freeQueue(buffer);

    exit(EXIT_SUCCESS);
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
        return 1;
    }

    filename = argv[argc-1];
    
    return 0;
}

int openPublicFIFO(char filename[]) {

    if (mkfifo(filename, 0777) < 0) {
        if (errno == EEXIST) fprintf(stderr, "FIFO '%s' already exists\n", filename);
        else {
            fprintf(stderr, "Can't create server FIFO!\n");
            return 1;
        }
    }

    if ((publicFifoFD = open(filename, O_RDONLY | O_NONBLOCK)) == -1) {
        fprintf(stderr, "Error in opening public FIFO\n");
        return 1;
    }

    struct pollfd fds;
    fds.fd = publicFifoFD;
    fds.events = POLLIN;
    while (!isServerClosed && (poll(&fds, 1, 0) != -1) && !(fds.revents & POLLIN)) {}   // TODO: poll error -> exit(EXIT_FAILURE)
    
    return 0;
}

void signalAlarmHandler(int signo) {
    isServerClosed = true;
    close(publicFifoFD);  
}
