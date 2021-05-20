#include "serverProducer.h"
#include "serverMain.h"
#include "log.h"
#include "lib.h"
#include "queue.h"
#include <pthread.h>

void * thread_producer(void * message) {
    Message *msg = (Message *) message;

    if (!isServerClosed) {
        // Get result from lib.h
        msg->tskres = task(msg->tskload);
        write_operation(*msg, TSKEX);
    } else msg->tskres = -1;    // Change result to -1 if serverIsClosed

    // Send message to buffer
    pthread_mutex_lock(&mutex);
    while(isFull(buffer)){};
    enqueue(buffer, *msg);
    pthread_mutex_unlock(&mutex);

    
    
    free(msg);
    pthread_exit(NULL);
}