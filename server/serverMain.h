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
 * @brief Size of Array threads
 */
int sizeOfThreads;

Queue buffer;

pthread_mutex_t mutex;

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

int verifyInput(int argc, char* argv[]);

int openPublicFIFO(char filename[]);

/**
 * @brief Handler of SIGALRM: purpose of of activating the isClientClosed flag
 * 
 * @param signo SIGALARM
 */
void signalAlarmHandler(int signo);

#endif  // SRC_SERVERMAIN_H_