//
// Created by capitalg on 2018/7/7.
//

#ifndef FILE_SYSTEM_SYSFILE_H
#define FILE_SYSTEM_SYSFILE_H

#include <structs/File.h>
#include <structs/Proc.h>
#include "Bcache.h"
#include "Icache.h"
#include "Ftable.h"
#include "Dir.h"

// SysFile 集合了所有关于文件操作的函数
class SysFile {
    Proc &cur_proc;
    Bcache &bcache;
    Icache &icache;
    Ftable &ftable;

    Dir &dir_util;



    File *argfd(int fd);
    Inode *create(const std::string &path, short type);


public:

    SysFile(Proc &cur_proc, Bcache &bcache, Icache &icache, Ftable &ftable, Dir &dir_util) : cur_proc(cur_proc),
                                                                                             bcache(bcache),
                                                                                             icache(icache),
                                                                                             ftable(ftable),
                                                                                             dir_util(dir_util) {}

    int fdalloc(File &f);

    /**
     * 如果 dup 的 fd对应的文件不存在，返回-1
     * @param fd
     * @return
     */
    int dup(int fd);

    int read(int fd, char* dest, int n);

    int write(int fd, char* src, int n);

    int close(int fd);

    /**
     * 创建链接
     * 创建一个名字叫 new_name 的链接文件，链接到叫名字叫 old_name的文件上
     * @param new_name
     * @param path
     * @return
     */
    int link(const std::string &new_name, const std::string &path);

    int unlink(const std::string &path);

    /**
     * 检测目录是否为空，空目录只包含"." 和".."
     * @param dir_inode
     * @return
     */
    bool isdirempty(Inode &dir_inode);


    /**
     * 返回打开文件的 fd
     * @param path
     * @param op
     * @return
     */
    int open(const std::string &path, open_option op);


    int mkdir(const std::string &path);

    int chdir(const std::string &path);

    
};


#endif //FILE_SYSTEM_SYSFILE_H
