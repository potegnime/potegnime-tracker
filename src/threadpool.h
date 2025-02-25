#include <stdint.h>
#include <pthread.h>

#ifndef THREADPOOL_H
#define THREADPOOL_H


typedef struct {
    pthread_t pid;
    
} workerinfo_t;


typedef struct {

    workerinfo_t* workers;
    char active;
    void (*success_callback); 

} threadpool_t;


typedef struct {


} Queue;

threadpool_t* threadpool_init(uint8_t workers);


void threadpool_enqueue_task(threadpool_t* pool, void* args);


void threadpool_dequeue_task();




void threadpool_destroy(threadpool_t* pool);

#endif