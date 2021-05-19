#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include "message.h"
 
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (ClientInfo*)malloc(queue->capacity * sizeof(ClientInfo));
    return queue;
}

void freeQueue(Queue* queue) {
    if (queue->array) free(queue->array);
    if (queue) free(queue);
}

int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

void enqueue(Queue* queue, ClientInfo item) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

ClientInfo dequeue(Queue* queue) {
    ClientInfo item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

ClientInfo front(Queue* queue) {
    return queue->array[queue->front];
}

ClientInfo rear(Queue* queue) {
    return queue->array[queue->rear];
}
