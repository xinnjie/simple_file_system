//
// Created by capitalg on 2018/7/4.
//

#include <iostream>
#include <util/panic.h>
#include "Bcache.h"

/*
 * 查找已经载入的buf 从 head 往后找
 * 查找空闲的 buf 从 head 往前找
 * 也就是说最近使用的磁盘缓冲块在链表头，不常使用的放在链表尾巴
 */
Buf &Bcache::bget(unsigned int dev, unsigned int blockno) {
    for (Buf *b = head.next;  b != &head ; b = b->next) {
        if (b->dev == dev && b->blockno == blockno) {
            b->refcnt++;
            return *b;
        }
    }

    // 找到一块不脏的磁盘进行替换
    for (Buf *b = head.prev;  b != &head ; b = b->prev) {
        if (b->refcnt == 0 && (b->flags & B_DIRTY) == 0) {
            b->dev = dev;
            b->blockno = blockno;
            b->flags = 0;
            b->refcnt = 1;
            return *b;
        }
    }

    // 替换一块脏磁盘块替换
    for (Buf *b = head.prev;  b != &head ; b = b->prev) {
        if (b->refcnt == 0 && (b->flags & B_DIRTY) != 0) {
            ideio.write(*b);
            b->dev = dev;
            b->blockno = blockno;
            b->flags = 0;
            b->refcnt = 1;
            return *b;
        }
    }
    std::cerr << "no free buf" << std::endl;

}

Buf &Bcache::bread(unsigned int dev, unsigned int blockno) {
    Buf &b = bget(dev, blockno);
    // 如果当前缓冲块不有效，需要重新读入
    if((b.flags & B_VALID) == 0) {
        ideio.read(b);
        b.flags |= B_VALID;
    }
    return b;
}

//todo 每次 bwrite 更新的内容直接被写入磁盘，是不是不太好
void Bcache::bwrite(Buf &buf) {
    buf.flags |= B_DIRTY;
    ideio.write(buf);
}

void Bcache::brelease(Buf &buf) {
    buf.refcnt--;
    Buf *bp = &buf;
    if (bp->refcnt == 0) {
        bp->next->prev = bp->prev;
        bp->prev->next = bp->next;
        bp->next = head.next;
        bp->prev = &head;
        head.next->prev = bp;
        head.next = bp;
    }
}

unsigned int Bcache::balloc(unsigned int dev) {
    // 在 bitmap 中查找空闲的块
    for (unsigned int block_index = 0; block_index < superBlock.size; block_index += BPB) {
        Buf &buf = bread(dev, locate_bitmap_block(block_index));
        for (int bitmap_index = 0; bitmap_index < BPB && block_index + bitmap_index < superBlock.size; ++bitmap_index) {
            int select_mask = 1 << (bitmap_index % 8);
            if ((buf.data[bitmap_index/8] & select_mask) == 0) {// 空闲块
                buf.data[bitmap_index/8] |= select_mask; // 置为已使用
                brelease(buf);
                bzero(dev, block_index + bitmap_index);
                return block_index + bitmap_index;
            }
        }
        brelease(buf);
    }

    panic("balloc: no blocks");
}

void Bcache::bzero(unsigned int dev, unsigned int block_index) {
    Buf &buf = bread(dev, block_index);
    memset(buf.data, 0, BSIZE);
    brelease(buf);
}
