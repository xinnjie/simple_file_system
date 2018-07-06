//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_DIR_H
#define FILE_SYSTEM_DIR_H


#include "Icache.h"

class Dir {
Icache &icache;

public:
    Dir(Icache &icache);

    /**
     * 在指定的目录 i 节点中查找对应名字的 i 节点指针，如果没有找到，返回 null_ptr
     * @param dir_inode
     * @param name
     * @return
     */
    Inode *dirlookup(Inode &dir_inode, std::string name);

    /**
     * 将指向 inum i节点的名字为 name 的目录项插入到 dir_inode 目录中
     * 如果目录项中已经含有了相同名字的目录项，返回-1，不然返回0
     * @param dir_inode
     * @param name
     * @param inum
     * @return
     */
    int insert_into_dir(Inode &dir_inode, std::string name, unsigned int inum);
};


#endif //FILE_SYSTEM_DIR_H
