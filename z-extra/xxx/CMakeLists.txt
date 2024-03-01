cmake_minimum_required(VERSION 3.17)
project(xxx C)

set(CMAKE_C_STANDARD 99)


set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(xxx
        library.c)