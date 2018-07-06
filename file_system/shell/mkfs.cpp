//
// Created by capitalg on 2018/7/5.
//

#include "mkfs.h"

void mkfs::make_file_system() {
    // 初始化 superblock

    init_superblock();
    // 将所有 metablocks 都标记为已经占用
    mark_in_use(1, 0, nmetablocks);

    // todo 根目录
    insert_root();

}



void mkfs::mark_in_use(unsigned int dev, unsigned int blockno) {
    unsigned int bitmap_block = blockno / BPB;
    unsigned int byte_index = (blockno % BPB) / 8;
    unsigned int bit_index = (blockno % BPB) % 8;

    Bcache &bcache = *bcache_p;
    Buf &buf = bcache.bread(dev, bitmap_block + superBlock.bmap_start);
    int select_mask = 1 << bit_index;
    buf.data[byte_index]  |= select_mask;
    bcache.bwrite(buf);
    bcache.brelease(buf);
}

void mkfs::mark_in_use(unsigned int dev, unsigned int from, unsigned int to) {
    for (unsigned int i = from; i < to; ++i) {
        mark_in_use(dev, i);
    }
}

mkfs::mkfs(std::string fs_name) {
    ideio_p = std::make_unique<IDEio>(fs_name, FSSIZE);
    bcache_p = std::make_unique<Bcache>(*ideio_p);
    icache_p = std::make_unique<Icache>(*bcache_p);
}

void mkfs::init_superblock() {
    unsigned int nbitmap = FSSIZE/(BSIZE*8) + 1;
    unsigned int ninodeblocks = NINODES / IPB + 1;

    nmetablocks = 2 + ninodeblocks + nbitmap;  // 预留0块 + 超级块 + inode块 + bitmap 块
    unsigned int nblocks = FSSIZE - nmetablocks;  // 数据块数量


    superBlock.size = FSSIZE;
    superBlock.n_inodes = NINODEBLOCKS;
    superBlock.inode_start = 2;
    superBlock.bmap_start = 2 + NINODEBLOCKS;
    superBlock.n_data = FSSIZE - nmetablocks;
    superBlock.data_start = nmetablocks;


    printf("nmeta %d (boot, super, inode blocks %u, bitmap blocks %u) blocks %d total %d\n",
           nmetablocks, ninodeblocks, nbitmap, nblocks, FSSIZE);
}

void mkfs::insert_root() {
    Icache &icache = *icache_p;
    Inode &root_inode = icache.ialloc(1, T_DIR);
    assert(root_inode.inum == 1);

}

