#include "ThreadPool.h"
#include "TaskQueue.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct ThreadPool {
    pthread_t* thread;
    size_t thread_count;
    TaskQueue* task_queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool active;
} ThreadPool;

static void* StartRoutine(void* TP)
{
    Task task;
    while(((ThreadPool*)TP)->active) {
        if(pthread_mutex_lock(&((ThreadPool*)TP)->mutex)) {
            return NULL;
        }
        if(pthread_cond_wait(&((ThreadPool*)TP)->cond, &((ThreadPool*)TP)->mutex)) {
            return NULL;
        }
        if(((ThreadPool*)TP)->task_queue->size) {
            task = TaskQueueFront(((ThreadPool*)TP)->task_queue);
            TaskQueueDequeue(((ThreadPool*)TP)->task_queue);
            task.function(task.argument);
        }
    }
    return NULL;
}

ThreadPool* ThreadPoolCreate(size_t capacity, size_t thread_count)
{
    ThreadPool* TP = (ThreadPool*)malloc(sizeof (ThreadPool));
    if(!TP) {
        return NULL;
    }
    TP->thread = (pthread_t*)malloc(sizeof (pthread_t) * thread_count);
    if(!TP->thread) {
        free(TP->task_queue);
        free(TP);
        return NULL;
    }
    for(size_t i = 0; i < thread_count; i++) {
        if(pthread_create(TP->thread + i, NULL, StartRoutine, TP)) {
            free(TP->task_queue);
            free(TP);
            return NULL;
        }
    }
    TP->task_queue = TaskQueueCreate(capacity);
    if(!TP->task_queue) {
        free(TP);
        return NULL;
    }
    if(pthread_mutex_init(&TP->mutex, NULL)) {
        free(TP->thread);
        free(TP->task_queue);
        free(TP);
        return NULL;
    }
	if(pthread_cond_init(&TP->cond, NULL)) {
        pthread_mutex_destroy(&TP->mutex);
        free(TP->thread);
        free(TP->task_queue);
        free(TP);
        return NULL;
    }
    TP->active = true;
    TP->thread_count = thread_count;
    return TP;
}

bool ThreadPoolDestroy(ThreadPool* TP)
{
    if(pthread_mutex_lock(&TP->mutex)) {
        return false;
    }
    if(pthread_cond_destroy(&TP->cond)) {
        return false;
    }
    if(pthread_mutex_destroy(&TP->mutex)) {
        return false;
    }
    TP->active = false;
    free(TP->thread);
    TaskQueueDestroy(TP->task_queue);
    free(TP);
    TP = NULL;
    return true;
}

bool ThreadPoolInsert(ThreadPool* TP, void (*function)(void*), void* argument)
{
    if(TP->task_queue->size == TP->task_queue->capacity) {
        return false;
    }
    if(pthread_mutex_lock(&TP->mutex)) {
        return false;
    }
    Task task;
    task.function = function;
    task.argument = argument;
    TaskQueueEnqueue(TP->task_queue, task);
    if(pthread_cond_signal(&TP->cond)) {
        return false;
    }
    if(pthread_mutex_unlock(&TP->mutex)) {
        return false;;
    }
    return true;
}

void ThreadPoolWait(ThreadPool* TP)
{
    return;
}

void ThreadPoolPause(ThreadPool* TP)
{
    return;
}

void ThreadPoolResume(ThreadPool* TP)
{
    return;
}
