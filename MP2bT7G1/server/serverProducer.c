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

        // Log action
        write_operation(*msg, TSKEX);
    }

    // Send message to buffer
    pthread_mutex_lock(&mutex);
    while (!enqueue(buffer, *msg)) {}
    pthread_mutex_unlock(&mutex);

    // Free dynamically allocated space for Message
    free(msg);

    pthread_exit(NULL);
}