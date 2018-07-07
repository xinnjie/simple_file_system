//
// Created by capitalg on 2018/7/5.
//

#ifndef FILE_SYSTEM_UTIL_H
#define FILE_SYSTEM_UTIL_H


#include <structs/SuperBlock.h>
#include <iostream>
#include "param.h"
#include "Bcache.h"


class Util {
public:

    static SuperBlock read_superblock(unsigned int dev, Bcache &bcache) {
        SuperBlock superBlock;
        Buf &buf = bcache.bread(dev, 1);
        memmove(&superBlock, buf.data, sizeof(superBlock));
        bcache.brelease(buf);
        return superBlock;
    }
    // [ boot block | super block | log | inode blocks | free bit map | data blocks]

    // [ 空闲块 | super block | inode blocks | free bit map | data blocks]

    static void fs_init(std::string fs_name) {
        IDEio ideio(fs_name, FSSIZE);
        Bcache bcache(ideio);
        SuperBlock superBlock;
        superBlock.size = FSSIZE;
        superBlock.n_inodes = NINODEBLOCKS;
        superBlock.inode_start = 2;
        superBlock.bmap_start = 2 + NINODEBLOCKS;
        superBlock.n_data = FSSIZE - 2 - NINODEBLOCKS - NBITMAP;
        superBlock.data_start = superBlock.bmap_start + NBITMAP;

        Buf &buf = bcache.bread(1,1);
        memmove(buf.data, &superBlock, sizeof(superBlock));
        bcache.bwrite(buf);
    }

};


#endif //FILE_SYSTEM_UTIL_H
