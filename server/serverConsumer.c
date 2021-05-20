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

    while (1) {
        if (!isEmpty(buffer)) {
            //Buscar um valor do buffer
            message = dequeue(buffer);
        
            
            snprintf(path, sizeof(path), "/tmp/%d.%ld",  message.pid, message.tid);

            //Modificar a mensagem
            message.pid = getpid();
            message.tid = pthread_self();

            if (isServerClosed && message.tskres != -1){
                write_operation(message,FAILD);
                continue;
            }

            fprintf(stderr, "1\n");

            if ((fifoDescriptor = open(path, O_WRONLY|O_NONBLOCK)) < 0) {
                write_operation(message, FAILD);
                fprintf(stderr,"Client is closed, cannot open private FIFO\n");
                continue;
            }

            fprintf(stderr,"2\n");

            //Enviar mensagem para um FIFO privado
            if (write(fifoDescriptor, &message, sizeof(message)) < 0) {
                fprintf(stderr, "Can't send message!\n");
                write_operation(message,FAILD);

            }
            
            if(message.tskres == -1) {
                write_operation(message,TLATE);
                if (close(fifoDescriptor) != 0) {
                    fprintf(stderr, "Can't close private FIFO\n");
                    exit(EXIT_FAILURE);
                }
                continue;
            }

            if (!isServerClosed) write_operation(message, TSKDN);
            else{write_operation(message,FAILD);}
            

            // Close private FIFO
            if (close(fifoDescriptor) != 0) {
                fprintf(stderr, "Can't close private FIFO\n");
                exit(EXIT_FAILURE);
            }

            fprintf(stderr,"3\n");
        }
    }

    
}