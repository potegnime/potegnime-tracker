#include "threadpool.h"
#include "logger.h"


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


static void* worker_job();

threadpool_t* threadpool_init(uint8_t num_workers) {

    for (uint16_t i = 0; i < num_workers; i++) {

    }   

}


void threadpool_destroy(threadpool_t* pool) {
    
}


static void* worker_job() {

}