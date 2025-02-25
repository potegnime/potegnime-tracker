#include <stdio.h>
#include "logger.h"

int main() {
    
    logger_initConsoleLogger(NULL);
    logger_initFileLogger("logs/log.txt", 1024 * 1024, 5);

    logger_setLevel(LogLevel_DEBUG);
    

    udp_init(6969);
    

    udp_deinit();

    return 0;
}