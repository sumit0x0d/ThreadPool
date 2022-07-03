#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct ThreadPool {
    pthread_t* thread;
    size_t thread_count;
    struct TaskQueue* task_queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ThreadPool;

ThreadPool* ThreadPool_create(size_t capacity, size_t thread_count);
bool ThreadPool_destroy(ThreadPool* TP);

bool ThreadPool_insert(ThreadPool* TP, void (*function)(void*), void* argument);
void ThreadPool_wait(ThreadPool* TP);
void ThreadPool_pause(ThreadPool* TP);
void ThreadPool_resume(ThreadPool* TP);

#endif
