cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
project(file_system_lib)



set(file_system_src
        IDEio.cpp IDEio.h structs/Buf.h param.h Bcache.cpp Bcache.h structs/SuperBlock.h structs/Inode.h structs/Dinode.h Icache.cpp Icache.h util/panic.cpp util/panic.h shell/mkfs.cpp shell/mkfs.h Dir.cpp Dir.h structs/DirEntry.h util/split_path.cpp util/split_path.h structs/File.h Ftable.cpp Ftable.h structs/FileStat.h structs/Proc.h SysFile.cpp SysFile.h util/read_superblock.h util/read_superblock.cpp shell/abstract_cmd.cpp shell/abstract_cmd.h shell/cd.cpp shell/cd.h shell/rm.cpp shell/rm.h shell/ls.cpp shell/ls.h shell/cat.cpp shell/cat.h shell/Shell.cpp shell/Shell.h shell/pwd.cpp shell/pwd.h shell/mkdir.cpp shell/mkdir.h shell/cp.cpp shell/cp.h shell/ln.cpp shell/ln.h shell/touch.cpp shell/touch.h util/file_exist.cpp util/file_exist.h shell/useradd.cpp shell/useradd.h shell/userdel.cpp shell/userdel.h structs/User.h shell/login.cpp shell/login.h util/split.cpp util/split.h util/get_unique_id.cpp util/get_unique_id.h shell/chmod.cpp shell/chmod.h shell/mv.cpp shell/mv.h shell/logout.cpp shell/logout.h)

add_library(file_system ${file_system_src})


add_executable(run_shell shell_main.cpp)
target_link_libraries(run_shell file_system)