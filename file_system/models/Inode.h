//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_INODE_H
#define FILE_SYSTEM_INODE_H

#include <param.h>

struct Inode {
    short type; // 区分文件、文件目录
    short nlink;  //  有多少个文件目录指向这个 i 节点
    unsigned int size;
    unsigned int addrs[NDIRECT+1];

    unsigned int dev;
    unsigned int inum;   // i 节点的 index
    int ref;   // 现在被进程打开多少次
    int valid;   // i 节点是否已经从磁盘读出
};
#endif //FILE_SYSTEM_INODE_H
