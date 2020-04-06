cmake_minimum_required(VERSION 3.13.0)
project(defuse)

set( SOURCES
    src/main.c
    src/password.c
    src/complex_wires.c
    src/input.c
    src/util.c
    src/bomb.c
    src/wires.c
)

SET( HEADERS
    inc/bomb.h
    inc/input.h
    inc/util.h
    src/apps.h
)

add_executable(defuse ${SOURCES} ${HEADERS})
add_compile_options(defuse -Wall -Wpedantic -Werror)

target_include_directories(defuse PRIVATE inc)
target_link_libraries(defuse PRIVATE pthread)
