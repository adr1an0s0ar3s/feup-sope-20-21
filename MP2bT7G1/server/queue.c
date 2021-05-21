#include "queue.h"
 
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

int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

int enqueue(Queue* queue, Message item) {
    if (isFull(queue)) return 0;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    return 1;
}

Message dequeue(Queue* queue) {
    Message item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

Message front(Queue* queue) {
    return queue->array[queue->front];
}

Message rear(Queue* queue) {
    return queue->array[queue->rear];
}
