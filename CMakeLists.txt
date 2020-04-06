cmake_minimum_required(VERSION 3.13.0)
project(defuse)

set( SOURCES
    src/main.c
    src/password.c
    src/complex_wires.c
    src/input.c
)

SET( HEADERS
    src/apps.h
    src/bomb.h
    src/input.h
)

add_executable(defuse ${SOURCES} ${HEADERS})
add_compile_options(defuse -Wall -Wpedantic -Werror)
target_link_libraries(defuse pthread)
