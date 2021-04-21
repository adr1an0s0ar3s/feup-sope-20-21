#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <sys/types.h>

/**
 * @brief Variable that stores the pid info of the process
 * 
 */
pid_t pid;

/**
 * @brief File descriptor of the public fifo
 * 
 */
int fdServerFifo;

/**
 * @brief Array containing all the threads
 * 
 */
pthread_t threads[10000]; 

/**
 * @brief Size of Array threads
 * 
 */
int sizeOfThreads;

/**
 * @brief Handler of SIGALRM: purpose of cleaning all threads;
 * 
 * @param signo 
 */
void signalAlarmHandler(int signo);

struct Arg_Thread{
    int request_id;
    int task;
};

#endif 
