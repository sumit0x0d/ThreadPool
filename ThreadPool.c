#include "ThreadPool.h"
#include "TaskQueue.h"
#include <stdlib.h>

ThreadPool* ThreadPool_create(size_t task_queue_capacity)
{
    ThreadPool* TP = (ThreadPool*)malloc(sizeof (ThreadPool));
    if(!TP) {
        return NULL;
    }
    TP->TQ.array = (Task*)malloc(task_queue_capacity);
    if(!TP->TQ.array) {
        free(TP);
        return NULL;
    }
    TP->TQ.front = 0;
    TP->TQ.back = 0;
    TP->TQ.capacity = task_queue_capacity;
    TP->TQ.size = 0;
    return TP;
}

void ThreadPool_destroy(ThreadPool* TP)
{

}

int ThreadPool_insert();
void ThreadPool_wait(ThreadPool* TP);
void ThreadPool_pause(ThreadPool* TP);
void ThreadPool_resume(ThreadPool* TP);
