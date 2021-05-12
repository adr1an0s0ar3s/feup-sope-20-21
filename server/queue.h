#ifndef SERVER_QUEUE_H_
#define SERVER_QUEUE_H_

// C program for array implementation of queue
//#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
 
// A structure to represent a queue
struct Queue {
    int front, rear, size;
    unsigned capacity;
    Message* array;
};
 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (Message*)malloc(
        queue->capacity * sizeof(Message));
    return queue;
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, Message item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}
 
// Function to remove an item from queue.
// It changes front and size
Message dequeue(struct Queue* queue)
{
    // TODO
    //if (isEmpty(queue))
    //    return INT_MIN;
    Message item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
Message front(struct Queue* queue)
{
    // TODO
    //if (isEmpty(queue))
    //    return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
Message rear(struct Queue* queue)
{
    // TODO
    //if (isEmpty(queue))
    //    return INT_MIN;
    return queue->array[queue->rear];
}

#endif  // SERVER_QUEUE_H_