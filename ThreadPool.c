#include "TaskQueue.h"
#include <stdbool.h>

typedef struct Thread {
    pthread_t pthread;
} Thread;

ThreadPool* ThreadPool_create(size_t capacity)
{
    ThreadPool* TP = (ThreadPool*)malloc(sizeof (ThreadPool));
    if(!TP) {
        return NULL;
    }
    TP->task_queue = TaskQueue_create(capacity);
    if(!TP->task_queue) {
        free(TP);
        return NULL;
    }
    return TP;
}

void ThreadPool_destroy(ThreadPool* TP)
{
    TaskQueue_destroy(TP->task_queue);
    free(TP);
    TP = NULL;
}

static Task* task_create(void (*function)(void* argument), void* argument)
{
    Task* task = (Task*)malloc(sizeof (Task));
    if(!task) {
        return NULL;
    }
    task->function = function;
    task->argument = argument;
    return task;
}

bool ThreadPool_insert(ThreadPool* TP, void (*function)(void* argument), void* argument)
{
    if(TP->task_queue->size == TP->task_queue->capacity) {
        return false;
    }
    Task* task = task_create(function, argument);
    if(!task) {
        return false;
    }
    TaskQueue_enqueue(TP->task_queue, task);
    return true;
}

void ThreadPool_wait(ThreadPool* TP)
{

}

void ThreadPool_pause(ThreadPool* TP)
{

}

void ThreadPool_resume(ThreadPool* TP)
{

}
