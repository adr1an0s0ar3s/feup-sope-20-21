#include "thread.h"
#include "message.h"
#include "client.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>

void * threadFunction(void * arg){
    //pthread_detach(pthread_self());
    struct Arg_Thread * args = (struct Arg_Thread *) arg;
    int fdFifo;

    // Create message
    Message msg;
    msg.rid = args->request_id;
    msg.pid = getpid();
    msg.tid = pthread_self();
    msg.tskload = args->task;
    msg.tskres = -1;
    
    // Create private FIFO
    char path[200];
    snprintf(path, 200, "/tmp/%d.%ld",  msg.pid, msg.tid);
    if (mkfifo(path, FIFO_MODE) != 0) {
        perror("Can't create thread!");
        exit(1);
    }

    // Send message
    if (write(fdServerFifo, &msg, sizeof(msg)) < 0) {
        perror("Can't send message!");
        exit(1);
    }
    
    // Receive response
    if ((fdFifo = open(path, O_RDONLY)) < 0) {
        printf("Can't open private FIFO!%d", msg.rid);
        exit(1);
    }

    if (read(fdFifo, &msg, sizeof(msg)) < 0) {
        perror("Can't recieve result!");
        exit(1);
    }

    //Log (mais tarde)

    //Desalocar recursos, fechar fifo privado e terminar
    if (close(fdFifo) != 0) {
        perror("Can't close private FIFO!");
        exit(1);
    }

    if (unlink(path) != 0) {
        perror("Can't erase private FIFO!");
        exit(1);
    }

    /*printf("%ld -- %d -- %d\n",pthread_self(),thread->request_id, thread->task );
    usleep(2000);*/
    pthread_exit(NULL);
}