#include <stdio.h>
#include "logger.h"

#include "udp_server.h"
#include "tracker_logic.h"

int main() {
    
    logger_initConsoleLogger(NULL);
    logger_initFileLogger("logs/log.txt", 1024 * 1024, 5);

    logger_setLevel(LogLevel_DEBUG);
    

    tracker_logic_init();

    tracker_add_user("1234553464fhshdthdak");
    tracker_add_user("1234h53j64fhshdthdak");
    tracker_add_user("1g345f34j4fhshkthdak");

    userinfo_t* user = tracker_get_user("1234553464fhshdthdak");
    tracker_remove_user("1234553464fhshdthdak");
    user = tracker_get_user("1234553464fhshdthdak");


    udp_init(6969);
    

    udp_deinit();

    return 0;
}