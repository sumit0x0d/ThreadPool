#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    void (*function)();
} Task;

typedef struct TaskQueue {
    Task* array;
    size_t front;
    size_t back;
    size_t capacity;
    size_t size;
} TaskQueue;

static inline TaskQueue* TaskQueue_create(size_t capacity)
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

static inline bool TaskQueue_change_capacity(TaskQueue* TQ, size_t capacity)
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

static inline void TaskQueue_destroy(TaskQueue* TQ)
{
    free(TQ->array);
    TQ->array = NULL;
    free(TQ);
    TQ = NULL;
}

static inline void* TaskQueue_front(TaskQueue* TQ)
{
    return TQ->array + TQ->front;
}

static inline void* TaskQueue_back(TaskQueue* TQ)
{
    return TQ->array + (TQ->back - 1);
}

static inline void TaskQueue_enqueue(TaskQueue* TQ, Task task)
{
    if(TQ->back == TQ->capacity) {
        memmove(TQ->array, TQ->array + TQ->front, TQ->size);
        TQ->front = 0;
        TQ->back = TQ->size;
    }
    TQ->array[TQ->back] = task;
    TQ->back = TQ->back + 1;
    TQ->size = TQ->size + 1;
}

static inline void TaskQueue_dequeue(TaskQueue* TQ)
{
    TQ->front = TQ->front + 1;
    TQ->size = TQ->size - 1;
}

#endif
