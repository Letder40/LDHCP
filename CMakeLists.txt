project(LDHCP)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)
file(MAKE_DIRECTORY build)

set(SOURCES
    src/main.c
    src/server/server.c
    src/server/vector.c
    src/transaction/request.c
    src/transaction/response.c
)

add_executable(ldhcp ${SOURCES})
