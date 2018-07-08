//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_READ_SUPERBLOCK_H
#define FILE_SYSTEM_READ_SUPERBLOCK_H

#include <Bcache.h>

class SuperBlock;
SuperBlock read_superblock(unsigned int dev, Bcache &bcache);
#endif //FILE_SYSTEM_READ_SUPERBLOCK_H
