//
// Created by capitalg on 2018/7/7.
//

#ifndef FILE_SYSTEM_PROC_H
#define FILE_SYSTEM_PROC_H

#include <vector>
#include "Inode.h"
#include "File.h"

/**
 * 并不是完整功能的 PCB，只包含了关于文件系统的功能
 */
class Proc {
public:
    Inode *cwdi;
    std::string cwd;
    std::vector<File*> open_files;


    /**
     * 最多同时打开 NOFILE 个文件
     */
     explicit Proc(Inode &pathi, const std::string &path): open_files(NOFILE), cwdi(&pathi), cwd(path) {}
};

#endif //FILE_SYSTEM_PROC_H
