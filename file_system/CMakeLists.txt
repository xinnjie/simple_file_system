cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
project(file_system_lib)



set(file_system_src
        main.cpp IDEio.cpp IDEio.h Buf.h param.h Bcache.cpp Bcache.h SuperBlock.h)

add_library(file_system ${file_system_src})