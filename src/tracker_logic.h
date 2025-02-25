#include <stdint.h>

#ifndef TRACKER_LOGIC_H
#define TRACKER_LOGIC

typedef struct {
    char info_hash[256];
    uint32_t seeders;
    uint32_t lecheers;
    uint32_t completed;

} torrentfile_t;

typedef struct {
    int64_t unique_id;
    uint32_t downloads;
    uint32_t uploads;
} userinfo_t;


void tracker_logic_init();


#endif
