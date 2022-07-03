#include "TaskQueue.h"

static void* thread_create(void* TP)
{
    Task task;
    for(;;) {
        if(pthread_mutex_lock(&((ThreadPool*)TP)->mutex)) {
            return NULL;
        }
        while(((ThreadPool*)TP)->active) {
            if(pthread_cond_wait(&((ThreadPool*)TP)->cond, &((ThreadPool*)TP)->mutex)) {
                return NULL;
            }
            if(((ThreadPool*)TP)->task_queue->size) {
                Task task = TaskQueue_front(((ThreadPool*)TP)->task_queue);
                TaskQueue_dequeue(((ThreadPool*)TP)->task_queue);
                task.function(task.argument);
            }
        }
    }
}

ThreadPool* ThreadPool_create(size_t capacity, size_t thread_count)
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
        if(pthread_create(TP->thread + i, NULL, thread_create, TP)) {
            free(TP->task_queue);
            free(TP);
            return NULL;
        }
    }
    TP->task_queue = TaskQueue_create(capacity);
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
        free(TP->thread);
        free(TP->task_queue);
        free(TP);
        return NULL;
    }
    TP->thread_count = thread_count;
    return TP;
}

bool ThreadPool_destroy(ThreadPool* TP)
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
    free(TP->thread);
    TaskQueue_destroy(TP->task_queue);
    free(TP);
    TP = NULL;
    return true;
}

bool ThreadPool_insert(ThreadPool* TP, void (*function)(void*), void* argument)
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
    TaskQueue_enqueue(TP->task_queue, task);
    if(pthread_cond_signal(&TP->cond)) {
        return false;
    }
    if(pthread_mutex_unlock(&TP->mutex)) {
        return false;;
    }
    return true;
}

void ThreadPool_wait(ThreadPool* TP)
{
    return;
}

void ThreadPool_pause(ThreadPool* TP)
{
    return;
}

void ThreadPool_resume(ThreadPool* TP)
{
    return;
}
