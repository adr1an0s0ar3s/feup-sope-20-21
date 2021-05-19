#include "serverConsumer.h"
#include "serverMain.h"
#include "log.h"
#include "queue.h"

#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void* thread_consumer(void * arg) {
    ClientInfo info;
    Message message;

    while (!isServerClosed || !isEmpty(buffer)) {
        if (!isEmpty(buffer)) {
            //Buscar um valor do buffer
            info = dequeue(buffer);
            message = info.msg;

            //Modificar a mensagem
            message.pid = getpid();
            message.tid = pthread_self();

            //Enviar mensagem para um FIFO privado
            if (write(info.fd, &message, sizeof(Message)) < 0) {
                write_operation(message, FAILD);
                //fprintf(stderr, "Can't send message!\n");
                printf("Here: %d\n%d\n%ld\n", info.fd, message.pid, message.tid);
                continue;
            }
            
            if (!isServerClosed) write_operation(message, TSKDN);
            else write_operation(message, TLATE);

            // Close private FIFO
            if (close(info.fd) != 0) {
                fprintf(stderr, "Can't close private FIFO\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    pthread_exit(NULL);
}