#include "serverProducer.h"
#include "serverMain.h"
#include "log.h"
#include "lib.h"
#include "queue.h"

void * thread_producer(void * message) {
    Message* msg = (Message*) message;
    while (!isServerClosed || !isFull(buffer)) {
        if (!isFull(buffer)) {
            msg->tskres = task(msg->tskload);
            if (isServerClosed) 
                msg->tskres = -1;
            write_operation(*msg, TSKEX);
            enqueue(buffer, *msg);
        }
    }
    free(msg);
    return NULL;
}