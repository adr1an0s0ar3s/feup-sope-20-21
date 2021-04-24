#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#include <sys/types.h>

typedef struct {
	int rid;  // request id
	pid_t pid;  // process id
	pthread_t tid;  // thread id
	int tskload;  // task load
	int tskres;  // task result
} Message;

#endif // SRC_MESSAGE_H_
