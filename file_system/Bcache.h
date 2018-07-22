//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_BCACHE_H
#define FILE_SYSTEM_BCACHE_H


#include <vector>
#include <structs/SuperBlock.h>
#include "structs/Buf.h"
#include "IDEio.h"

class Bcache {
private:
    std::vector<Buf> bufs;
    Buf head;
    SuperBlock superBlock;

    IDEio &ideio;


    /**
     * 返回第 index 块磁盘块所在的 bitmap 的块号
     * @param index
     * @return
     */
    inline unsigned int locate_bitmap_block(unsigned int index) {
        return index/BPB + superBlock.bmap_start;
    }


public:

    /**
     * bget 找到对应的磁盘缓冲块返回
     *  通过 dev, blockno 找到对应的磁盘缓冲块
     *  如果该块没有被缓冲，会应用 LRU 规则腾出一块空闲的缓冲块，返回该块（但仍然未读入，需要后续读入）
     * @param dev
     * @param blockno
     * @return
     */
    Buf &bget(unsigned int dev, unsigned int blockno);

    /**
     *  通过链表实现的 LRU
     */
    explicit Bcache(IDEio &ideio): bufs(NBUF), head(), ideio(ideio){
        head.prev = &head;
        head.next = &head;
        for (auto it = bufs.begin(); it != bufs.end(); ++it) {
            it->next = head.next;
            it->prev = &head;
            head.next->prev = &(*it);
            head.next = &(*it);
        }

        superBlock = read_superblock(1);
    }

    /**
     * 返回对应的磁盘缓冲块，假如还未缓冲，会从磁盘读入
     * @param dev
     * @param blockno
     * @return
     */
    Buf &bread(unsigned int dev, unsigned int blockno);
    /*
     * 将该磁盘缓冲块写入，但不会立刻写入
     * 考虑分别写入多次，不必每次写入都 flush 磁盘，而是都写入磁盘缓冲块，最后 flush
     * flush 的时机是该磁盘块离开缓冲块
     */
    void bwrite(Buf &buf);

    /**
     * 每次释放的磁盘缓冲块放在链表头，表示最近被使用，优先链表尾的磁盘缓冲块（这些块很长时间没有被使用）
     * @param buf
     */
    void brelease(Buf &buf);

    /**
     * 从磁盘中中分配出一块空闲块，会把对应磁盘 bitmap 置为已经使用，返回磁盘块号，可以用该磁盘号获取磁盘缓冲块
     * @param dev
     * @return
     */
    unsigned int balloc(unsigned int dev);

    /**
     * 使指定的块变为空闲块
     * @param dev
     * @param blockno
     */
    void bfree(unsigned dev, unsigned int blockno);

    /**
     * 将指定的 block 置为零
     * @param dev
     * @param block_index
     */
    void bzero(unsigned int dev, unsigned int block_index);

    void setSuperBlock(const SuperBlock &superBlock);

    const SuperBlock &getSuperBlock() const;


    SuperBlock read_superblock(unsigned int dev) {
        SuperBlock superBlock;
        Buf &buf = bread(dev, SUPERBLOCK_INDEX);
        memmove(&superBlock, buf.data, sizeof(superBlock));
        brelease(buf);
        return superBlock;
    }

};



#endif //FILE_SYSTEM_BCACHE_H
