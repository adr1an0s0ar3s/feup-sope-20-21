#include "serverProducer.h"
#include "message.h"
#include "serverMain.h"
#include "log.h"
#include "lib.h"
#include "queue.h"
#include <pthread.h>

void * thread_producer(void *message) {

    ClientInfo info = *(ClientInfo *) message;

    // Changing the result, otherwise it's equal to -1
    if (!isServerClosed) {

        // Get result from lib.h
        info.msg.tskres = task(info.msg.tskload);

        // Log the action
        write_operation(info.msg, TSKEX);
    }

    // Send message to buffer
    pthread_mutex_lock(&mutex);
    while (enqueue(buffer, info) == 1) {}
    pthread_mutex_unlock(&mutex);
   
    pthread_exit(NULL);
}