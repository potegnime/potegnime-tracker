#include "threadpool.h"
#include "logger.h"

static pthread_mutex_t mutex;


static void* worker_job();

threadpool_t* threadpool_init(uint8_t num_workers) {

    threadpool_t* pool = malloc(sizeof(threadpool_t));

    pool->workers = calloc(num_workers, sizeof(workerinfo_t));

    for(uint8_t i = 0; i < num_workers; i++) {
        pthread_create(&pool->workers[i].pid, NULL, worker_job, NULL);
        pthread_detach(&pool->workers[i].pid);
    }

    

}


void threadpool_destroy(threadpool_t* pool) {
    
}


static void* worker_job() {
    while(1) {
        
    }
}