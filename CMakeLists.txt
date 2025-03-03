cmake_minimum_required(VERSION 3.16)

project(traker VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(EVENT__DISABLE_TESTS ON CACHE BOOL "Disable libevent tests")
set(EVENT__DISABLE_BENCHMARK ON CACHE BOOL "Disable libevent benchmarks")
set(EVENT__DISABLE_REGRESS ON CACHE BOOL "Disable libevent regress tests")

add_subdirectory(libevent)

set(SOURCES 
    src/main.c
    src/logger.c
    src/threadpool.c
    src/udp_server.c
    src/tracker_logic.c
    src/hashmap.c
    src/http_server.c
)


find_package(Threads REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(traker ${SOURCES})


target_link_libraries(traker PRIVATE Threads::Threads event_extra PostgreSQL::PostgreSQL)

target_include_directories(traker PRIVATE ${PROJECT_SOURCE_DIR}/src ${LIBEVENT_INCLUDE_DIR})