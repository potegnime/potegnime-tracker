#include <stdint.h>

#ifndef TRACKER_LOGIC_H
#define TRACKER_LOGIC

typedef struct torrentfile_t {
    uint32_t seeders;
    uint32_t lecheers;
    uint32_t completed;

} torrentfile_t;

typedef struct userinfo_t {
    char peer_id[20];
    uint32_t downloads;
    uint32_t uploads;
} userinfo_t;




void tracker_logic_init();

void tracker_add_user(const char* unique_id);
void tracker_add_torrent(const char* info_hash);


void tracker_remove_user(const char* unique_id);
void tracker_remove_torrent(const char* info_hash);


userinfo_t* tracker_get_user(const char* unique_id);
torrentfile_t* tracket_get_torrent(const char* info_hash);

#endif
