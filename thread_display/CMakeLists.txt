cmake_minimum_required(VERSION 3.12)
project(GTKfirst C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

add_definitions(${GTK2_CFLAGS_OTHER})

add_executable(GTKfirst main.c window_functions.c window_functions.h)

target_link_libraries(GTKfirst ${GTK2_LIBRARIES})

#find_package(Threads)
#target_link_libraries(GTKfirst ${CMAKE_THREAD_LIBS_INIT})