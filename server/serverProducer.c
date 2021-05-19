#include "serverProducer.h"
#include "serverMain.h"
#include "log.h"
#include "lib.h"
#include "queue.h"
#include <pthread.h>

void * thread_producer(void * message) {
    Message *msg = (Message *) message;

    while (!isServerClosed || !isFull(buffer)) {
        if (!isFull(buffer)) {
            // Get result from lib.h
            msg->tskres = task(msg->tskload);

            // Change result to -1 if serverIsClosed
            if (isServerClosed) msg->tskres = -1;
            else write_operation(*msg, TSKEX);

            // Send message to buffer
            pthread_mutex_lock(&mutex);
            enqueue(buffer, *msg);
            pthread_mutex_unlock(&mutex);
        }
    }
    
    free(msg);
    pthread_exit(NULL);
}