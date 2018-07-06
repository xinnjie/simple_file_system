//
// Created by capitalg on 2018/7/5.
//
#include "shell.h"
#include <param.h>
#include <models/SuperBlock.h>
#include <cstdio>

// [ 空闲块 | super block | inode blocks | free bit map | data blocks]



/**
 * 将 metablock 在初始情况下都标记为已经占用
 * @param from 
 * @param to 
 */
void mark_in_use(unsigned int from, unsigned int to) {
    
}


void mkfs() {
    unsigned int nbitmap = FSSIZE/(BSIZE*8) + 1;
    unsigned int ninodeblocks = NINODES / IPB + 1;

    unsigned int nmetablocks = 2 + ninodeblocks + nbitmap;  // 预留0块 + 超级块 + inode块 + bitmap 块
    unsigned int nblocks = FSSIZE - nmetablocks;  // 数据块数量


    SuperBlock superBlock;
    superBlock.size = FSSIZE;
    superBlock.n_inodes = NINODEBLOCKS;
    superBlock.inode_start = 2;
    superBlock.bmap_start = 2 + NINODEBLOCKS;
    superBlock.n_data = FSSIZE - nmetablocks;
    superBlock.data_start = nmetablocks;

    printf("nmeta %d (boot, super, inode blocks %u, bitmap blocks %u) blocks %d total %d\n",
           nmetablocks, ninodeblocks, nbitmap, nblocks, FSSIZE);

    // 将所有 metablocks 都标记为已经占用


}