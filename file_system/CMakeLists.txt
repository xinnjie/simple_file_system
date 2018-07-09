cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(file_system_lib)



set(file_system_src
        main.cpp IDEio.cpp IDEio.h structs/Buf.h param.h Bcache.cpp Bcache.h structs/SuperBlock.h structs/Inode.h structs/Dinode.h Icache.cpp Icache.h util/panic.cpp util/panic.h shell/mkfs.cpp shell/mkfs.h Dir.cpp Dir.h structs/DirEntry.h util/split_path.cpp util/split_path.h structs/File.h Ftable.cpp Ftable.h structs/FileStat.h structs/Proc.h SysFile.cpp SysFile.h util/read_superblock.h util/read_superblock.cpp shell/abstract_cmd.cpp shell/abstract_cmd.h shell/cd.cpp shell/cd.h shell/rm.cpp shell/rm.h shell/ls.cpp shell/ls.h shell/format.cpp shell/format.h shell/cat.cpp shell/cat.h shell/Shell.cpp shell/Shell.h shell/pwd.cpp shell/pwd.h shell/mkdir.cpp shell/mkdir.h)

add_library(file_system ${file_system_src})


add_executable(run_shell shell_main.cpp)
target_link_libraries(run_shell file_system)