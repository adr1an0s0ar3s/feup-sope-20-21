#include "serverConsumer.h"
#include "serverMain.h"
#include "log.h"
#include "queue.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void * thread_consumer(void *arg) {

    ClientInfo info;
    Message message;
    char path[200];

    while (!isServerClosed || !isEmpty(buffer)) {
        if (!isEmpty(buffer)) {

            // Dequeue valie from the Queue
            info = dequeue(buffer);
            message = info.msg;

            fprintf(stderr, "%d\n", message.tskres);

            snprintf(path, sizeof(path), "/tmp/%d.%ld",  info.msg.pid, info.msg.tid);

            //Modificar a mensagem
            message.pid = getpid();
            message.tid = pthread_self();

            if ((info.fd = open(path, O_WRONLY)) < 0) {
                write_operation(message, FAILD);
                fprintf(stderr, "Client is closed, cannot open private FIFO\n");
                continue;
            }

            if (isServerClosed) {
                write_operation(message, TLATE);
                message.tskres = -1;
            }

            printf("Result: %d\n", message.tskres);

            if (write(info.fd, &message, sizeof(Message)) < 0) {
                write_operation(message, FAILD);
                fprintf(stderr, "Cannot write on private fifo\n");
            }
            
            write_operation(message, TSKDN);

            // Close private FIFO
            if (close(info.fd) != 0) {
                fprintf(stderr, "Can't close private FIFO\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    pthread_exit(NULL);
}