cmake_minimum_required(VERSION 3.12)
project(proc_analysis C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

add_executable(proc_analysis main.c functions.c functions.h)

target_link_libraries(proc_analysis ${GTK2_LIBRARIES})