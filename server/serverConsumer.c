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
    char path[200];

    while (!isServerClosed || !isEmpty(buffer)) {
        if (!isEmpty(buffer)) {
            //Buscar um valor do buffer
            info = dequeue(buffer);
            message = info.msg;

            snprintf(path, sizeof(path), "/tmp/%d.%ld",  info.msg.pid, info.msg.tid);
              //Modificar a mensagem
            message.pid = getpid();
            message.tid = pthread_self();

            if ((info.fd = open(path, O_WRONLY)) < 0) {
                write_operation(message,FAILD);
                //fprintf(stderr,"Client is closed, cannot open private FIFO\n");
                continue;
            }
          

            if(message.tskres == -1){
                write_operation(message, TLATE);
                close(info.fd);
                continue;
            }

            

            if(write(info.fd,&message,sizeof(Message))<0){
                fprintf(stderr,"Cannot write on private fifo\n");
                write_operation(message, FAILD);
                continue;
            }

            if (!isServerClosed) write_operation(message, TSKDN);

            // Close private FIFO
            if (close(info.fd) != 0) {
                fprintf(stderr, "Can't close private FIFO\n");

                exit(EXIT_FAILURE);
            }
            

            

    
        }
    }

    pthread_exit(NULL);
}