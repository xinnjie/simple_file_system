//
// Created by capitalg on 2018/7/4.
//

#include <structs/SuperBlock.h>
#include <structs/Buf.h>
#include <structs/Dinode.h>
#include <iostream>
#include <util/panic.h>
#include <util/read_superblock.h>
#include "Icache.h"
#include "Bcache.h"

using namespace std;

unsigned int Icache::inode_location(int inum) {
    return (inum/IPB + superBlock.inode_start);
}



unsigned int Icache::find_block_location(Inode &inode, unsigned int bn) {
    //  当 bn 小于 NDIRECT 上采用直接寻址，大于 NDIRECT 时需要间接寻址
    unsigned int addr = 0;
    if (bn < NDIRECT) {
        // 如果该块还没有被分配，立刻进行分配
        if ((addr = inode.addrs[bn]) == 0) {
            inode.addrs[bn] = addr = bcache.balloc(inode.dev);
            return addr;
        }
        return addr;
    }
    bn -= NDIRECT;

    if (bn < NINDIRECT) {
        if ((addr = inode.addrs[NINDIRECT]) == 0) {  // 间接寻址项还没有被分配块
            inode.addrs[NDIRECT] = addr = bcache.balloc(inode.dev);
        }
        Buf &buf = bcache.bread(inode.dev, addr);
        unsigned int *entry = reinterpret_cast<unsigned int*>(buf.data);
        if ((addr = entry[bn]) == 0) {
            entry[bn] = addr = bcache.balloc(inode.dev);
        }
        bcache.brelease(buf);
        return addr;
    }

    panic("find_block_location: out of range, file too big");
}


void Icache::read_dinode(Inode &inode) {
    if (inode.valid == 0) {
        Buf &buf = bcache.bread(inode.dev, inode_location(inode.inum));
        Dinode *dinode = reinterpret_cast<Dinode*>(buf.data + (inode.inum % IPB)* sizeof(Dinode));
        inode.type = dinode->type;
        inode.nlink = dinode->nlink;
        inode.size = dinode->size;
        memmove(inode.addrs, dinode->addrs, sizeof(inode.addrs));
        bcache.brelease(buf);
        inode.valid = 1;
        if (inode.type == 0) {
            panic("read_dinode: no type");
        }
    }
}



Icache::Icache(Bcache &cache):inodes(NINODE), bcache(cache), superBlock(cache.getSuperBlock()) {
}

Inode &Icache::ialloc(unsigned int dev, short type) {
    // inum = 0不使用
    for (int inum = 1; inum < superBlock.n_inodes; ++inum) {
        Buf &buf = bcache.bread(dev, inode_location(inum));
        Dinode *dinode = reinterpret_cast<Dinode*>(buf.data + (inum%IPB)* sizeof(Dinode));
        if (dinode->type == 0) {  // free
            memset(dinode, 0, sizeof(*dinode));
            dinode->type = type;
            bcache.bwrite(buf);
            bcache.brelease(buf);
            return iget(dev, inum);
        }
        bcache.brelease(buf);
    }

    panic("ialloc: inode alloc fail");

}

Inode &Icache::iget(unsigned int dev, unsigned int inum) {
    Inode *empty = nullptr;

    for (auto it = inodes.begin(); it != inodes.end(); ++it) {
        if (it->ref > 0 && it->dev == dev && it->inum == inum) {
            it->ref++;
            return *it;
        }

        if (empty == nullptr && it->ref == 0) {
            empty = &(*it);
        }
    }

    // 走到这里说明该 i 节点没有被载入缓存中
    if (empty == nullptr) {
        panic("iget: no inodes cache");
    }
    empty->dev = dev;
    empty->inum = inum;
    empty->ref = 1;
    empty->valid = 0;

    read_dinode(*empty);
    return *empty;

}

// fixme 特殊情况
void Icache::iput(Inode &inode) {
//    if (inode.valid && inode.nlink == 0) {
//        if (inode)
//    }

    inode.ref--;
}

void Icache::iupdate(Inode &inode) {
    Buf &buf = bcache.bread(inode.dev, inode_location(inode.inum));
    Dinode *dinode = reinterpret_cast<Dinode*>(buf.data + (inode.inum%IPB)* sizeof(Dinode));
    dinode->type = inode.type;
    dinode->nlink = inode.nlink;
    dinode->size = inode.size;
    dinode->uid = inode.uid;
    dinode->prl = inode.prl;
    memmove(dinode->addrs, inode.addrs, sizeof(inode.addrs));
    bcache.bwrite(buf);
    bcache.brelease(buf);
}

int Icache::readi(Inode &inode, char *dest, unsigned int off, unsigned int n) {
    assert(off >= 0);
    // 检查
    if (off > inode.size ||  n < 0) {
        return -1;
    }
    if (off + n > inode.size) {
        n = inode.size - off;
    }

    unsigned int total_read = 0, read_once = 0;
    for (total_read = 0; total_read < n; total_read += read_once, dest += read_once) {
        Buf &buf = bcache.bread(inode.dev, find_block_location(inode, off / BSIZE));
        read_once = min(n - total_read, BSIZE - (off % BSIZE));
        memmove(dest, buf.data + (off%BSIZE), read_once);
        bcache.brelease(buf);
    }
    return total_read;
}

int Icache::writei(Inode &inode, const char *src, unsigned int off, unsigned int n) {
    assert(off >= 0);
    if (off > inode.size) {
        return -1;
    }
    if (off + n > MAXFILE*BSIZE) {
        return  -1;
    }

    unsigned int total_written = 0, write_once = 0;
    for (; total_written < n; total_written += write_once, off += write_once, src += write_once) {
        Buf &buf = bcache.bread(inode.dev, find_block_location(inode, off / BSIZE));
        write_once = min(n-total_written, BSIZE - (off%BSIZE));
        memmove(buf.data + (off%BSIZE), src, write_once);
        bcache.bwrite(buf);
        bcache.brelease(buf);
    }

    // 如果文件比原来大，需要更新 i 节点的信息
    if (n > 0 && off > inode.size) {
        inode.size = off;
        iupdate(inode);
    }
    return total_written;
}

void Icache::idelete(Inode &inode) {
    // 释放直接寻址的磁盘块
    for (unsigned int i = 0; i < NDIRECT; ++i) {
        if (inode.addrs[i]) {
            bcache.bfree(inode.dev, inode.addrs[i]);
            inode.addrs[i] = 0;
        }
    }

    // 释放间接寻址的磁盘块, 包括释放间接引用的磁盘块和本身间接寻址要占用的那一块磁盘块
    if (inode.addrs[NDIRECT]) {
        Buf &buf = bcache.bread(inode.dev, inode.addrs[NDIRECT]);
        unsigned int *p = reinterpret_cast<unsigned int*>(buf.data);
        for (int i = 0; i < NINDIRECT; ++i) {
            if (p[i]) {
                bcache.bfree(inode.dev, p[i]);
            }
        }
        bcache.brelease(buf);
        bcache.bfree(inode.dev, inode.addrs[NDIRECT]);
        inode.addrs[NDIRECT] = 0;
    }
    inode.size = 0;
    iupdate(inode);

}

Inode &Icache::idup(Inode &inode) {
    inode.ref++;
    return inode;
}
