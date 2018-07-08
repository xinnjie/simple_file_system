//
// Created by capitalg on 2018/7/8.
//
#include <structs/Buf.h>
#include <cstring>
#include "read_superblock.h"

SuperBlock read_superblock(unsigned int dev, Bcache &bcache) {
    SuperBlock superBlock;
    Buf &buf = bcache.bread(dev, 1);
    memmove(&superBlock, buf.data, sizeof(superBlock));
    bcache.brelease(buf);
    return superBlock;
}