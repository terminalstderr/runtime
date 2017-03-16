cmake_minimum_required(VERSION 2.8)
project(runtime C)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -D_GNU_SOURCE")


set(SOURCE_FILES
    Builtins.c
    Builtins.h
    sample_quackmain.c)

add_executable(runtime ${SOURCE_FILES})
