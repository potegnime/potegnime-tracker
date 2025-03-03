#include <stdio.h>
#include "logger.h"

#include "udp_server.h"
#include "tracker_logic.h"
#include "http_server.h"

int main() {
    
    logger_initConsoleLogger(NULL);
    logger_initFileLogger("logs/log.txt", 1024 * 1024, 5);

    logger_setLevel(LogLevel_DEBUG);
    

    tracker_logic_init();
    http_server_init();


    //udp_init(6969);

    //udp_deinit();

    return 0;
}