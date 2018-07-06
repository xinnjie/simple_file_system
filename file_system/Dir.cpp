//
// Created by capitalg on 2018/7/6.
//

#include <models/DirEntry.h>
#include <util/panic.h>
#include "Dir.h"

Inode * Dir::dirlookup(Inode &dir_inode, std::string name) {
    unsigned int offset = 0;
    DirEntry dir_entry;
    if (dir_inode.type != T_DIR) {
        panic("dirlookup: not dir");
    }
    for (offset = 0; offset < dir_inode.size; offset += sizeof(DirEntry)) {
        if (icache.readi(dir_inode, reinterpret_cast<char*>(&dir_entry), offset, sizeof(dir_entry)) != sizeof(dir_entry)) {
            panic("dirlookup: incomplete read");
        }
        if (dir_entry.inum == 0) { // 该块为空
            continue;
        }
        if (dir_entry.name == name) {
            unsigned int inum = dir_entry.inum;
            return &icache.iget(dir_inode.dev, inum);
        }
    }
    return nullptr;
}

Dir::Dir(Icache &icache) : icache(icache) {}

int Dir::insert_into_dir(Inode &dir_inode, std::string name, unsigned int inum) {
    if (dir_inode.type != T_DIR) {
        panic("insert_into_dir: not dir");
    }

    if (dirlookup(dir_inode, name) != nullptr) {
        return -1;
    }

    // 找到空闲的目录项
    unsigned int offset = 0;
    DirEntry dir_entry;
    for (offset = 0; offset < dir_inode.size; offset += sizeof(DirEntry)) {
        if (icache.readi(dir_inode, reinterpret_cast<char*>(&dir_entry), offset, sizeof(DirEntry)) != sizeof(DirEntry)) {
            panic("insert_into_dir: read incompelte");
        }
        if (dir_entry.inum == 0) {
            break;
        }
    }
    assert(name.size() < DIR_NAME_SZ);
    strcpy(dir_entry.name, name.c_str());
    dir_entry.inum = inum;
    if (icache.writei(dir_inode, reinterpret_cast<char*>(&dir_entry),  offset, sizeof(DirEntry)) != sizeof(DirEntry)) {
        panic("insert_into_dir: write dir entry failed");
    }
    return 0;
}
