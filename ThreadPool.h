#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct TaskQueue TaskQueue;

typedef struct ThreadPool {
    TaskQueue* task_queue;
    size_t thread_count;
    // size_t
} ThreadPool;

ThreadPool* ThreadPool_create(size_t capacity);
void ThreadPool_destroy(ThreadPool* TP);

bool ThreadPool_insert(ThreadPool* TP, void (*function)(void* arguments), void* arguments);
void ThreadPool_wait(ThreadPool* TP);
void ThreadPool_pause(ThreadPool* TP);
void ThreadPool_resume(ThreadPool* TP);

#endif
