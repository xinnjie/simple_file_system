cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
project(file_system_lib)



set(file_system_src
        main.cpp IDEio.cpp IDEio.h models/Buf.h param.h Bcache.cpp Bcache.h models/SuperBlock.h models/Inode.h models/Dinode.h Icache.cpp Icache.h Util.cpp Util.h util/panic.cpp util/panic.h)

add_library(file_system ${file_system_src})