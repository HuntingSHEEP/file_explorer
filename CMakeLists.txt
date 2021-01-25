cmake_minimum_required(VERSION 3.17)
project(file_explorer C)

set(CMAKE_C_STANDARD 11)

add_executable(file_explorer
        dis1.h
        gt4.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(file_explorer ${GTK3_LIBRARIES})