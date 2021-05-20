#ifndef SRC_SERVERMAIN_H_
#define SRC_SERVERMAIN_H_

#include "queue.h"
#include <sys/types.h>

/**
* @brief Mutex used to manage the access to the buffer.
*/
pthread_mutex_t mutex;

/**
* @brief Buffer that contains the messages
*/
Queue *buffer;

/**
 * @brief Flag which tells the program if the server is still running.
 * If is it equal to 1, the server has ended and vice-versa
 */
int isServerClosed;

/**
 * @brief File descriptor of the public fifo
 */
int publicFifoFD;

/**
 * @brief Number of threads created
 */
int n_threads;

/**
 * @brief Verifies the input
 * 
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int, 0 if the input is correct, 1 otherwise
 */
int verifyInput(int argc, char* argv[]);

/**
 * @brief Opens the public server FIFO
 * 
 * @param filename Public FIFO filename
 * @return int, 0 if success, 1 otherwise
 */
int openPublicFIFO(char filename[]);

/**
 * @brief Handler of SIGALRM: to activate the isClientClosed flag and close publicFifo
 * 
 * @param signo SIGALARM
 */
void signalAlarmHandler(int signo);

#endif  // SRC_SERVERMAIN_H_