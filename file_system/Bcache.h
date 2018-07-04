//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_BCACHE_H
#define FILE_SYSTEM_BCACHE_H


#include <vector>
#include "Buf.h"
#include "IDEio.h"

class Bcache {
private:
    std::vector<Buf> bufs;
    Buf head;

    IDEio &ideio;
    /**
     * bget 为内部使用的方法，找到对应的磁盘缓冲块返回
     *  通过 dev, blockno 找到对应的磁盘缓冲块
     *  如果该块没有被缓冲，会应用 LRU 规则腾出一块空闲的缓冲块，返回该块（但仍然未读入，需要后续读入）
     * @param dev
     * @param blockno
     * @return
     */
    Buf &bget(unsigned int dev, unsigned int blockno);

public:
    /**
     *  通过链表实现的 LRU
     */
    Bcache(IDEio &ideio): bufs(NBUF), head(), ideio(ideio){
        head.prev = &head;
        head.next = &head;
        for (auto it = bufs.begin(); it != bufs.end(); ++it) {
            it->next = head.next;
            it->prev = &head;
            head.next->prev = &(*it);
            head.next = &(*it);
        }
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

};



#endif //FILE_SYSTEM_BCACHE_H
