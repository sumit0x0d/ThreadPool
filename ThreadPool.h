#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ThreadPool ThreadPool;

ThreadPool* ThreadPoolCreate(size_t capacity, size_t thread_count);
bool ThreadPoolDestroy(ThreadPool* TP);

bool ThreadPoolInsert(ThreadPool* TP, void (*function)(void*), void* argument);
void ThreadPoolWait(ThreadPool* TP);
void ThreadPoolPause(ThreadPool* TP);
void ThreadPoolResume(ThreadPool* TP);

#endif
