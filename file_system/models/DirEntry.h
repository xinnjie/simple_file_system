//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_DIR_ENTRY_H
#define FILE_SYSTEM_DIR_ENTRY_H

#include <param.h>

// 每个 DirEntry 占16字节
struct DirEntry {
    unsigned int inum;  // 这个目录项指向哪个文件 i 节点
    char name[DIR_NAME_SZ];
};
#endif //FILE_SYSTEM_DIR_ENTRY_H
