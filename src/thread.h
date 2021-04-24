#ifndef SRC_THREAD_H_
#define SRC_THREAD_H_

#define FIFO_MODE 0777

/**
 * @brief Creates, sends message to publicFifo and recieves result through a private FIFO
 * 
 * @param arg, Struct with request_id and respective task
 */
void * threadFunction(void * arg);

#endif  // SRC_THREAD_H_