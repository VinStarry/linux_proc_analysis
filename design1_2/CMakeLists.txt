cmake_minimum_required(VERSION 3.13)
project(design1_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(design1_2 main.c)

find_package(Threads)
target_link_libraries(design1_2 ${CMAKE_THREAD_LIBS_INIT})