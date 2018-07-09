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

    /**
     * 创建文件或目录
     * @param path
     * @param type 指明文件 T_FILE 或者 目录 T_DIR
     * @return
     */
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

    int write(int fd, const char *src, int n);

    /**
     * 关闭文件，关闭失败返回-1
     * @param fd
     * @return
     */
    int close(int fd);

    /**
     * 创建链接
     * 创建一个名字叫 new_name 的链接文件，链接到叫名字叫 old_name的文件上
     * @param new_name
     * @param path
     * @return
     */
    int link(const std::string &new_name, const std::string &path);

    /**
     * 删除链接，把对应的 i 节点引用计数-1, 当引用计数到达0时，会把 i 节点置零（其中类型设为0，表示空闲），达到删除文件的效果
     * @param path
     * @return
     */
    int unlink(const std::string &path);

    /**
     * 检测目录是否为空，空目录只包含"." 和".."
     * @param dir_inode
     * @return
     */
    bool isdirempty(Inode &dir_inode);


    /**
     * 返回打开文件的 fd, 打开失败返回-1
     * @param path
     * @param op
     * @return
     */
    int open(const std::string &path, open_option op);


    /**
     * 创建成功返回0，创建失败返回-1
     * @param path
     * @return
     */
    int mkdir(const std::string &path);

    /**
     * 切换成功返回0，切换失败返回-1
     * @param path
     * @return
     */
    int chdir(const std::string &path);

};


#endif //FILE_SYSTEM_SYSFILE_H
