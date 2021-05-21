#include "serverConsumer.h"
#include "serverMain.h"
#include "log.h"
#include "queue.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void * thread_consumer(void *arg) {

    Message message;
    int fifoDescriptor;
    char path[200];

    while (!isServerClosed || n_threads != 1 || !isEmpty(buffer)) {
        if (!isEmpty(buffer)) {

            // Gets a Message from buffer
            message = dequeue(buffer);
            
            snprintf(path, sizeof(path), "/tmp/%d.%ld",  message.pid, message.tid);

            // Modifying message
            message.pid = getpid();
            message.tid = pthread_self();

            if ((fifoDescriptor = open(path, O_WRONLY | O_NONBLOCK)) == -1) {   // Check if errno == ENXIO?
                write_operation(message, FAILD);
                fprintf(stderr, "Client is closed, cannot open private FIFO\n");
            } else {

                // Sending Message to private FIFO
                if (write(fifoDescriptor, &message, sizeof(message)) == -1) {
                    write_operation(message, FAILD);
                    fprintf(stderr, "Client is closed, cannot write on private FIFO\n");
                } else if (message.tskres == -1) {
                    write_operation(message, TLATE);
                } else {
                    write_operation(message, TSKDN);
                }

                // Close private FIFO
                if (close(fifoDescriptor) != 0) {
                    fprintf(stderr, "Can't close private FIFO\n");
                    exit(EXIT_FAILURE);     // Perhaps it could be better?
                }
            }
        }
    }

    pthread_exit(NULL);
}