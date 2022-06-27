#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "TaskQueue.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct Thread {
    pthread_t pthread;
} Thread;

typedef struct ThreadPool {
    TaskQueue TQ;
    size_t task_queue_capacity;
    size_t threads;
} ThreadPool;

ThreadPool* ThreadPool_create(size_t task_queue_capacity);
void ThreadPool_destroy(ThreadPool* TP);

int ThreadPool_insert();
void ThreadPool_wait(ThreadPool* TP);
void ThreadPool_pause(ThreadPool* TP);
void ThreadPool_resume(ThreadPool* TP);

#endif
