cmake_minimum_required(VERSION 3.4)
project(runtime)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Builtins.c
    Builtins.h
    sample_quackmain.c)

add_executable(runtime ${SOURCE_FILES})