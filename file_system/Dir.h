//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_DIR_H
#define FILE_SYSTEM_DIR_H


#include <structs/DirEntry.h>
#include <structs/Proc.h>
#include "Icache.h"

class Dir {
Icache &icache;


    /**
        * 如果 get_child 设置为 true，child_name 不被设置，直接返回 path 对应的 i 节点
        * 如果 get_child 被设置为 false, child_name 将被设置为 path 的最后一层名字， 返回父目录对应的 i节点
        * example：path_name = "a/bb/c"
        *          true: 返回 c 的 inode
        *          false: 返回 a/bb 的 inode，并设置 name=“c”
        * @param path
        * @param child_name
        * @param through_out
        * @return
        */
    Inode *namex(const std::string &path, std::string *child_name, bool through_out);

public:
    Dir(Icache &icache, Proc &cur_proc);

    /**
     * 在指定的目录 i 节点中查找对应名字的 i 节点指针，如果没有找到，返回 null_ptr
     * @param dir_inode
     * @param name
     * @return
     */
    std::pair<Inode*, int> dirlookup(Inode &dir_inode, const std::string &name);

    std::vector<DirEntry> dirls(Inode &dir_inode);

    /**
     * 将指向 inum i节点的名字为 name 的目录项插入到 dir_inode 目录中
     * 如果目录项中已经含有了相同名字的目录项，返回-1，不然返回0
     * @param dir_inode
     * @param file_name
     * @param file_inum
     * @return
     */
    int dirlink(Inode &dir_inode, const std::string &file_name, unsigned int file_inum);


    Inode *namei(const std::string &path);

    /**
     *
     * @param path
     * @param child_name
     * @return
     */
    std::pair<Inode *, std::string> nameiparent(const std::string &path);

    Proc &cur_proc;
};


#endif //FILE_SYSTEM_DIR_H
