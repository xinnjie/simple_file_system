//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_BUF_H
#define FILE_SYSTEM_BUF_H

#include "param.h"

struct Buf {
    int flags; // flag给出内存和磁盘的联系：B_VALID表示数据已经被读入，B_DIRTY 表示数据需要被写出
    unsigned int dev;  // dev blockno 给出设备号和扇区号
    unsigned int blockno;
    unsigned int refcnt;
    Buf *prev;
    Buf *next;
    unsigned char data[BSIZE]; // 每个扇区包含512字节，data 为这512字节的缓存

};

#define B_VALID 0x2  // 磁盘缓冲块已经从磁盘中读出
#define B_DIRTY 0x4  // 磁盘缓冲被修改过，需要被写入磁盘

#endif //FILE_SYSTEM_BUF_H
