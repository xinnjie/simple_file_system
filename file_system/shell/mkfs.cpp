//
// Created by capitalg on 2018/7/5.
//

#include <structs/DirEntry.h>
#include <util/panic.h>
#include "mkfs.h"

using namespace std;
void mkfs::make_file_system() {
    // 初始化 superblock

    init_superblock();
    // 将所有 metablocks 都标记为已经占用
    mark_in_use(ROOTDEV, 0, nmetablocks);

    insert_root();
    insert_userinfo();

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

mkfs::mkfs(std::string fs_name, bool overwrite) {
    ideio_p = make_unique<IDEio>(fs_name, FSSIZE, overwrite);
    bcache_p = make_unique<Bcache>(*ideio_p);
    icache_p = make_unique<Icache>(*bcache_p);
}

void mkfs::init_superblock() {
    Bcache &bcache = *bcache_p;
    Icache icache = *icache_p;
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

    Buf &superBuf = bcache.bget(ROOTDEV, SUPERBLOCK_INDEX);
    memmove(superBuf.data, &superBlock, sizeof(superBlock));
    bcache.bwrite(superBuf);
    bcache.setSuperBlock(superBlock);


    printf("nmeta %d (boot, super, inode blocks %u, bitmap blocks %u) blocks %d total %d \n\n",
           nmetablocks, ninodeblocks, nbitmap, nblocks, FSSIZE);
}

void mkfs::insert_root() {
    Icache &icache = *icache_p;
    Inode &root_inode = icache.ialloc(ROOTDEV, T_DIR);
    assert(root_inode.inum == ROOTINO);
    assert(root_inode.dev == ROOTDEV);

    DirEntry dir_entry{};
    dir_entry.inum =  ROOTINO;
    strcpy(dir_entry.name, ".");

    if (icache.writei(root_inode, reinterpret_cast<char*>(&dir_entry), sizeof(dir_entry)*0, sizeof(dir_entry)) !=
            sizeof(dir_entry)) {
        panic("insert_root: insert . failed");
    }

    strcpy(dir_entry.name, "..");
    if (icache.writei(root_inode, reinterpret_cast<char*>(&dir_entry), sizeof(dir_entry)*1, sizeof(dir_entry)) !=
        sizeof(dir_entry)) {
        panic("insert_root: insert .. failed");
    }
    root_inode.nlink = 1;
    icache.iupdate(root_inode);
    icache.iput(root_inode);
}

void mkfs::insert_userinfo() {
    Inode &root = icache_p->iget(ROOTDEV, ROOTINO);
    User user(0, "root", "root");
    cur_proc_p = make_unique<Proc>(root, "/", *icache_p, user);
    dir_p = make_unique<Dir>(*icache_p, *cur_proc_p);
    ftable_p = make_unique<Ftable>(*icache_p);
    sysfile_p = make_unique<SysFile>(*cur_proc_p, *bcache_p, *icache_p, *ftable_p, *dir_p);
    SysFile &sysfile = *sysfile_p;
    assert(sysfile.mkdir("/users") != -1);
    assert(sysfile.mkdir("/users/root") != -1);
    assert(sysfile.mkdir("/etc") != -1);
    int fd = 0;
    fd = sysfile.open("/etc/.passwd", open_option::CREATE);
    char *root_info = "0 root root|";
    assert(sysfile.write(fd, root_info, strlen(root_info) + 1) == (strlen(root_info) + 1));

    assert(fd != -1);
    assert(sysfile.close(fd) != -1) ;

}

