#include "serverProducer.h"
#include "serverMain.h"
#include "log.h"
#include "lib.h"
#include "queue.h"
#include <pthread.h>

void * thread_producer(void * message) {
    ClientInfo info = *(ClientInfo *) message;


    if (!isServerClosed) {
        // Get result from lib.h
        info.msg.tskres = task(info.msg.tskload);
        write_operation(info.msg, TSKEX);
    } else info.msg.tskres = -1;    // Change result to -1 if serverIsClosed

    // Send message to buffer
    pthread_mutex_lock(&mutex);
    while(enqueue(buffer, info)==1){}
    pthread_mutex_unlock(&mutex);

   
    pthread_exit(NULL);
}