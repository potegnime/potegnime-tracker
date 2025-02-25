#include "tracker_logic.h"

#include "logger.h"

#include <pthread.h>

static pthread_mutex_t mutex;


void tracker_logic_init() {
    
    int r;
    if ((r = pthread_mutex_init(&mutex, NULL)) != 0) {
        LOG_FATAL("pthread_mutex_init(): %d", r);
        return;
    }

}
