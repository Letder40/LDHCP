project(MiProyecto C)
set(CMAKE_C_COMPILER gcc)
file(MAKE_DIRECTORY build)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SOURCES
    src/main.c
    src/server/server.c
    src/server/server_pool.c
    src/transaction/request.c
)

add_executable(ldhcp ${SOURCES})
