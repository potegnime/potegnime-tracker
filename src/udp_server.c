#include "udp_server.h"

#include "logger.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <pthread.h>

#define MSG_CONNECT 0
#define MSG_ANNOUNCE 1
#define MSG_SCRAPE 2
#define MSG_ERROR 3


#pragma pack(push, 1)

struct payload {
    int64_t connection_id;
    uint32_t action;
} payload;

struct connection_request {
    int64_t protocol_id;
    uint32_t action;
    int32_t transaction_id;

} connection_request;

struct connection_response {
    uint32_t action;
    int32_t transaction_id;
    int64_t connection_id;
} connection_response;


typedef struct {
    uint8_t type;
    uint8_t size;
    char data[256];
} extension_url_data_t;

struct announce_request {
    int64_t connection_id;
    uint32_t action;
    int32_t transaction_id;
    char info_hash[20];
    char peer_id[20];
    uint64_t downloaded;
    uint64_t left;
    uint64_t uploaded;
    int32_t event;
    int32_t ip_address;
    int32_t key;
    int32_t num_want;
    uint16_t port;
    extension_url_data_t url_data;
} announce_request;

struct announce_response {
    uint32_t action;
    int32_t transaction_id;
    uint32_t interval;
    uint32_t leechers;
    uint32_t seeders;

    //od kle naprj ipji in port ip:port

} announce_response;

struct scrape_request {
    int64_t protocol;
    uint32_t action;
    int32_t transaction_id;
    char info_hash[20];
} scrape_request;

struct scrape_response {
    uint32_t action;
    int32_t transaction_id;
} scrape_response;

#pragma pack(pop)

static int sockfd;
static pthread_t thread_worker;


//Za delanje connection id-ja. eni random byti
static char key[] = {0x32, 0xad, 0x43, 0xf2, 0x56, 0xab, 0x86, 0x12};


//definicije
static void* udp_server_worker();
static void make_connection_id(struct sockaddr_in* addr, char* dest);

//handlerji
static void handle_connect(struct sockaddr_in* addr, struct connection_request* req);
static void handle_announce(struct sockaddr_in* addr, struct announce_request* req);
static void handle_scrape(struct sockaddr_in* addr, struct scrape_request* req);

void handle_request(struct sockaddr_in* addr, const char* data, uint16_t size);


void udp_init(uint16_t port) {
    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    memset(serv_addr.sin_zero, '\0', sizeof serv_addr);


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        LOG_FATAL("socket(): %d", sockfd);
        return;
    }

    int r = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof serv_addr);
    if (r < 0) {
        LOG_FATAL("bind(): %d", r);
        return;
    }

    char ipstr[INET_ADDRSTRLEN];

    pthread_create(&thread_worker, NULL, udp_server_worker, NULL);
    LOG_INFO("Started udp server. Listening on: %s:%u", inet_ntop(serv_addr.sin_family, &serv_addr.sin_addr, ipstr, sizeof ipstr),port);
    
}

static void* udp_server_worker() {
    
    char buf[1024];
    struct sockaddr_storage peer_addr;
    int num_bytes;
    socklen_t addr_size = sizeof peer_addr;

    char ipstr[INET_ADDRSTRLEN];

    while(1) {

        num_bytes = recvfrom(sockfd, buf, sizeof buf, 0, (struct sockaddr *)&peer_addr, &addr_size);
        if (num_bytes == -1) {
            LOG_ERROR("recvfrom()");
            continue;
        }
        

        LOG_DEBUG("Receiving from IP address: %s:%u", inet_ntop(peer_addr.ss_family, &((struct sockaddr_in *)&peer_addr)->sin_addr, ipstr, sizeof ipstr), ((struct sockaddr_in *)&peer_addr)->sin_port);
        handle_request((struct sockaddr_in *)&peer_addr, buf, num_bytes);
    }

    pthread_exit(NULL);
}


void handle_request(struct sockaddr_in* addr, const char* data, uint16_t size) {
    struct payload* req = (struct payload*)data;
    
    //to je prot spoofingu ip-ja
    if ( req->action != MSG_CONNECT) {
        int64_t connec_id = 0;
        make_connection_id(addr, (char*)&connec_id);
        if (req->connection_id != connec_id)
            return;    
    }

    switch (req->action) {
    case MSG_CONNECT:
        if (size < 16)
            break;
        handle_connect(addr, (struct connection_request*)data);
        break;
    case MSG_ANNOUNCE:
        if (size < 20)
            break;
        handle_announce(addr, (struct announce_request*)data);
        break;
    case MSG_SCRAPE:
        if (size < 8)
            break;
        handle_scrape(addr, (struct scrape_request*)data);
        break;
    default:
        break;
    }
}


static void make_connection_id(struct sockaddr_in* addr, char* dest) {
    for(uint8_t i = 0; i < 8; i++) {
        dest[i] = (((uint8_t*)&addr->sin_addr.s_addr)[i % 4]) ^ key[i];
        dest[i] = dest[i] ^ ((uint8_t*)&addr->sin_port)[i % 2];
    }
}


int64_t swap_int64( int64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

static void handle_connect(struct sockaddr_in* addr, struct connection_request* req) {
    int64_t t = swap_int64(req->protocol_id);

    if (swap_int64(req->protocol_id) != 0x41727101980)
        return;
    
    

}

static void handle_announce(struct sockaddr_in* addr, struct announce_request* req) {

}

static void handle_scrape(struct sockaddr_in* addr, struct scrape_request* req) {

}

void udp_deinit() {
    pthread_join(thread_worker, NULL);
    close(sockfd);
}

