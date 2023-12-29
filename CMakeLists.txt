project(LDHCP)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
file(MAKE_DIRECTORY build)

set(SOURCES
    src/main.c
    src/server/server.c
    src/server/server_pool.c
    src/transaction/request.c
)

add_executable(ldhcp ${SOURCES})
