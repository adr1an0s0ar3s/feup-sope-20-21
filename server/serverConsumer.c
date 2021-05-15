#include "serverConsumer.h"
#include "serverMain.h"
#include "log.h"
#include "queue.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void* thread_consumer(void * arg) {
    Message message;
    int fifoDescriptor;
    char path[200];

    while (!isServerClosed || !isEmpty(buffer)) {
        if (!isEmpty(buffer)) {
            //Buscar um valor do buffer
            message = dequeue(buffer);
            
            snprintf(path, sizeof(path), "/tmp/%d.%ld",  message.pid, message.tid);

            //Modificar a mensagem
            message.pid = getpid();
            message.tid = pthread_self();

            if ((fifoDescriptor = open(path, O_WRONLY)) < 0) {
                write_operation(message, FAILD);
                fprintf(stderr,"Client is closed, cannot open private FIFO\n");
                continue;
            }

            //Enviar mensagem para um FIFO privado
            if (write(fifoDescriptor, &message, sizeof(message)) < 0) {
                fprintf(stderr, "Can't send message!\n");
                exit(EXIT_FAILURE);
            }
            
            if (!isServerClosed) write_operation(message, TSKDN);
            else write_operation(message, TLATE);

            // Close private FIFO
            if (close(fifoDescriptor) != 0) {
                fprintf(stderr, "Can't close private FIFO\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    pthread_exit(NULL);
}