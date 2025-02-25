cmake_minimum_required(VERSION 3.16)

project(traker VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES 
    src/main.c
    src/logger.c
    src/threadpool.c
    src/udp_server.c
    src/tracker_logic.c
)

find_package(Threads REQUIRED)

add_executable(traker ${SOURCES})
target_link_libraries(traker Threads::Threads)
target_include_directories(traker PRIVATE ${PROJECT_SOURCE_DIR}/src)