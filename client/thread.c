#include "thread.h"

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>

#include "message.h"
#include "client.h"
#include "log.h"

// Testing
extern int n_IWANT, n_CLOSD, n_GOTRS, n_GAVUP;

void * threadFunction(void * arg) {

    static int threadCount = 0;
    
    int fdFifo;

    // Create message
    Message msg;
    msg.rid = ++threadCount;
    msg.pid = getpid();
    msg.tid = pthread_self();
    msg.tskload = rand_r(&seed) % 9 + 1;
    msg.tskres = -1;
    
    // Create private FIFO
    char path[200];
    snprintf(path, sizeof(path), "/tmp/%d.%ld",  msg.pid, msg.tid);
    if (mkfifo(path, FIFO_MODE) != 0) {
        fprintf(stderr, "Can't create private FIFO!\n");
        exit(EXIT_FAILURE);
    }

    // Send message
    if (write(publicFifoFD, &msg, sizeof(msg)) < 0) {
        fprintf(stderr, "Can't send message!\n");
        exit(EXIT_FAILURE);
    }

    write_operation(msg, IWANT);
    n_IWANT++;

    if (!isClientClosed) {
        // Receive response
        if ((fdFifo = open(path, O_RDONLY)) < 0) {
            fprintf(stderr, "Can't open private FIFO! %d\n", msg.rid);
            exit(EXIT_FAILURE);
        }

        if (read(fdFifo, &msg, sizeof(msg)) < 0) {
            fprintf(stderr, "Can't receive result!\n");
            exit(EXIT_FAILURE);
        }

        if (msg.tskres != -1) {
            write_operation(msg, GOTRS);
            n_GOTRS++;
        } else {
            isServerClosed = true;
            write_operation(msg, CLOSD);
            n_CLOSD++;
        }

        // Close private FIFO
        if (close(fdFifo) != 0) {
            fprintf(stderr, "Can't close private FIFO!\n");
            exit(EXIT_FAILURE);
        }

    } else {
        write_operation(msg, GAVUP);
        n_GAVUP++;
    }

    if (unlink(path) != 0) {
        fprintf(stderr, "Can't erase private FIFO!\n");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL); // Or return
}