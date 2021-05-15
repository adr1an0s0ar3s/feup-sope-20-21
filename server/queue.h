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
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (Message*)malloc(queue->capacity * sizeof(Message));
    return queue;
}

void freeQueue(Queue* queue) {
    if (queue->array) free(queue->array);
    if (queue) free(queue);
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(Queue* queue) {
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(Queue* queue, Message item) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
 
// Function to remove an item from queue.
// It changes front and size
Message dequeue(Queue* queue) {
    Message item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
Message front(Queue* queue) {
    return queue->array[queue->front];
}
 
// Function to get rear of queue
Message rear(Queue* queue) {
    return queue->array[queue->rear];
}

#endif  // SERVER_QUEUE_H_