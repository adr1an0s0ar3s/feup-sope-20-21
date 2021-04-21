#ifndef MESSAGE_H
#define MESSAGE_H
typedef struct {
	int rid;  // request id
	pid_t pid;  // process id
	pthread_t tid;  // thread id
	int tskload;  // task load
	int tskres;  // task result
} Message;
#endif // MESSAGE_H
