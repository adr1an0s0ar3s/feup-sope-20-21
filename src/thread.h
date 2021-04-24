#ifndef THREAD_H
#define THREAD_H

#define FIFO_MODE 0777

/**
 * @brief Creates, sends message to publicFifo and recieves result through a private FIFO
 * 
 * @param arg, Struct with request_id and respective task
 */
void * threadFunction(void * arg);

#endif // THREAD_H