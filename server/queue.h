#ifndef SERVER_QUEUE_H_
#define SERVER_QUEUE_H_

// C program for array implementation of queue
//#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
 
// A structure to represent a queue
typedef struct {
    int front, rear, size;
    unsigned capacity;
    Message* array;
} Queue;
 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(unsigned capacity);

void freeQueue(Queue* queue);
 
// Queue is full when size becomes
// equal to the capacity
int isFull(Queue* queue);
 
// Queue is empty when size is 0
int isEmpty(Queue* queue);
 
// Function to add an item to the queue.
// It changes rear and size
int enqueue(Queue* queue, Message item);
 
// Function to remove an item from queue.
// It changes front and size
Message dequeue(Queue* queue);
 
// Function to get front of queue
Message front(Queue* queue);
 
// Function to get rear of queue
Message rear(Queue* queue);

#endif  // SERVER_QUEUE_H_