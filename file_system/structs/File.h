//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_FILE_H
#define FILE_SYSTEM_FILE_H

struct Inode;

enum class open_option {
    READ_ONLY,
    WRITE_ONLY,
    WRITE_READ,
    CREATE
};

enum class fd_type {
    FD_NONE,
    FD_INODE
};

/**
 * 文件描述符层， 提供编程接口，整合 pipe（以后再加）
 */
struct File {
    fd_type type;
    int ref;    //当前文件被打开了几次
    bool readable;
    bool writable;
    Inode *ip;
    unsigned int offset;  // 打开文件的当前偏移
};
#endif //FILE_SYSTEM_FILE_H
