project(MiProyecto C)
set(CMAKE_C_COMPILER gcc)

set(SOURCES
    ../src/main.c
    ../src/server/server.c
    ../src/server/server_pool.c
    ../src/transaction/request.c
)

add_executable(ldhcp ${SOURCES})
