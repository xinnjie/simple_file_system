//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_DINODE_H
#define FILE_SYSTEM_DINODE_H

#include <param.h>
#include <cassert>

/*
 * 每个磁盘 i 节点占64字节，每个磁盘块刚好可以装8个磁盘 i 节点
 */
struct Dinode {
    short type; // 区分文件、文件目录
    short nlink;  //  有多少个文件目录指向这个 i 节点
    unsigned int size;
    unsigned int addrs[NDIRECT+1];
    int padding;
};


//assert(sizeof(Dinode) == SIZE_DINODE)
#endif //FILE_SYSTEM_DINODE_H
