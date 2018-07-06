cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(file_system_lib)



set(file_system_src
        main.cpp IDEio.cpp IDEio.h models/Buf.h param.h Bcache.cpp Bcache.h models/SuperBlock.h models/Inode.h models/Dinode.h Icache.cpp Icache.h Util.cpp Util.h util/panic.cpp util/panic.h shell/mkfs_.cpp shell/shell.h shell/mkfs.cpp shell/mkfs.h Dir.cpp Dir.h models/DirEntry.h)

add_library(file_system ${file_system_src})