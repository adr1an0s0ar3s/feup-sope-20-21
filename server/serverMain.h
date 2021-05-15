#ifndef SRC_SERVERMAIN_H_
#define SRC_SERVERMAIN_H_

#include "queue.h"
#include <sys/types.h>

/**
 * @brief File descriptor of the public fifo
 */
int publicFifoFD;

/**
 * @brief Thread id of the main client
 */
pthread_t daddy_thread;

/**
* @brief Buffer that contains the messages
*/
Queue *buffer;

/**
* @brief Mutex used to manage the access to the buffer.
*/
pthread_mutex_t mutex;

/**
 * @brief Flag which tells the program if the server is still running.
 * If is it equal to 1, the server has ended and vice-versa
 */
int isServerClosed;

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int verifyInput(int argc, char* argv[]);

/**
 * @brief Opens the public server FIFO
 * 
 * @param filename Public FIFO
 * @return int Checks if the functione went well
 */
int openPublicFIFO(char filename[]);

/**
 * @brief Handler of SIGALRM: purpose of of activating the isClientClosed flag
 * 
 * @param signo SIGALARM
 */
void signalAlarmHandler(int signo);

#endif  // SRC_SERVERMAIN_H_