//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_SUPERBLOCK_H
#define FILE_SYSTEM_SUPERBLOCK_H
struct SuperBlock {
    unsigned int size;  // 文件系统总磁盘块个数
    unsigned int data_blocks;
    unsigned int inode_blocks;
    unsigned int inode_start;
    unsigned int bmap_start;
};
#endif //FILE_SYSTEM_SUPERBLOCK_H
