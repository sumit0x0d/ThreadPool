#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "stdlib.h"
#include "string.h"

typedef struct Task {
    void (*function)(void* argument);
    void* argument;
} Task;

typedef struct TaskQueue {
    Task* array;
    size_t front;
    size_t back;
    size_t capacity;
    size_t size;
} TaskQueue;

static inline TaskQueue* TaskQueueCreate(size_t capacity)
{
    TaskQueue* TQ = (TaskQueue*)malloc(sizeof (TaskQueue));
    if(!TQ) {
        return NULL;
    }
    TQ->array = (Task*)malloc(sizeof (Task) * capacity);
    if(!TQ->array) {
        free(TQ);
        return NULL;
    }
    TQ->front = 0;
    TQ->back = 0;
    TQ->capacity = capacity;
    TQ->size = 0;
    return TQ;
}

static inline bool TaskQueueChangeCapacity(TaskQueue* TQ, size_t capacity)
{
    Task* array = (Task*)realloc(TQ->array, capacity);
    if(!array) {
        return false;
    }
    TQ->array = array;
    TQ->capacity = capacity;
    if(TQ->capacity < TQ->size) {
        TQ->back = TQ->capacity;
        TQ->size = TQ->capacity;
    }
    return true;
}

static inline void TaskQueueDestroy(TaskQueue* TQ)
{
    free(TQ->array);
    TQ->array = NULL;
    free(TQ);
    TQ = NULL;
}

static inline Task TaskQueueFront(TaskQueue* TQ)
{
    return TQ->array[TQ->front];
}

static inline void TaskQueueEnqueue(TaskQueue* TQ, Task task)
{
    if(TQ->back == TQ->capacity) {
        memmove(TQ->array, TQ->array + TQ->front, TQ->size);
        TQ->front = 0;
        TQ->back = TQ->size;
    }
    TQ->array[TQ->back] = task;
    TQ->back++;
    TQ->size++;
}

static inline void TaskQueueDequeue(TaskQueue* TQ)
{
    TQ->front++;
    TQ->size--;
}

#endif
