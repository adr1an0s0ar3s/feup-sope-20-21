#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

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
 * @brief Size of Array threads
 */
int sizeOfThreads;

/**
 * @brief Flag which tells the program if the client is still running.
 * If is it equal to 1, the client has ended and vice-versa
 * 
 */
int isClientClosed;

/**
 * @brief Flag which tells the program if the server is still running.
 * If is it equal to 1, the server has ended and vice-versa
 */
int isServerClosed;

/**
 * @brief Seed used to generate random numbers
 */
unsigned int seed;

/**
 * @brief Handler of SIGALRM: purpose of of activating the isClientClosed flag
 * 
 * @param signo SIGALARM
 */
void signalAlarmHandler(int signo);

#endif  // SRC_CLIENT_H_
