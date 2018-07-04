//
// Created by capitalg on 2018/7/4.
//

#include <iostream>
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
