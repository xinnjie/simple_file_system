//
// Created by capitalg on 2018/7/6.
//

#include <structs/DirEntry.h>
#include <util/panic.h>
#include <util/split_path.h>
#include "Dir.h"

using namespace std;

std::pair<Inode*, int> Dir::dirlookup(Inode &dir_inode, const string &name) {
    unsigned int offset = 0;
    DirEntry dir_entry{};
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
            return make_pair(&icache.iget(dir_inode.dev, inum), offset);
        }
    }
    return make_pair(nullptr, -1);
}

Dir::Dir(Icache &icache, Inode *cwd) : icache(icache) {}

int Dir::dirlink(Inode &dir_inode, const string &file_name, unsigned int file_inum) {
    if (dir_inode.type != T_DIR) {
        panic("dirlink: not dir");
    }

    if (dirlookup(dir_inode, file_name).first != nullptr) {
        return -1;
    }

    // 找到空闲的目录项
    unsigned int offset = 0;
    DirEntry dir_entry;
    for (offset = 0; offset < dir_inode.size; offset += sizeof(DirEntry)) {
        if (icache.readi(dir_inode, reinterpret_cast<char*>(&dir_entry), offset, sizeof(DirEntry)) != sizeof(DirEntry)) {
            panic("dirlink: read incompelte");
        }
        if (dir_entry.inum == 0) {
            break;
        }
    }
    assert(file_name.size() < DIR_NAME_SZ);
    strcpy(dir_entry.name, file_name.c_str());
    dir_entry.inum = file_inum;
    if (icache.writei(dir_inode, reinterpret_cast<char*>(&dir_entry),  offset, sizeof(DirEntry)) != sizeof(DirEntry)) {
        panic("dirlink: write dir entry failed");
    }
    return 0;
}

Inode *Dir::namex(const std::string &path, string *child_name, bool through_out) {
    Inode *ip, *next;
    string nextpath = path;
    if (path[0] == '/') {
        ip = &icache.iget(ROOTDEV, ROOTINO);
        nextpath = path.substr(1);
    } else {
        assert(cwd != nullptr);
        ip = &icache.idup(*cwd);
    }

    auto root_and_rest = split_path(nextpath);
    string root = root_and_rest.first;
    string rest = root_and_rest.second;
    while (!root.empty()) {
        // 如果当前 i 节点不是目录，说明输入的文件路径有问题
        if (ip->type != T_DIR) {
            icache.iput(*ip);
            return nullptr;
        }
        if (!through_out && rest.empty()) {
            *child_name = root;
            return ip;
        }
        next = dirlookup(*ip, root).first;
        icache.iput(*ip);
        ip = next;
        root_and_rest = split_path(rest);
        root = root_and_rest.first;
        rest = root_and_rest.second;
    }
    if (!through_out) {
        icache.iput(*ip);
        return nullptr;
    }
    return ip;


}

Inode *Dir::namei(const string &path) {
    return namex(path, nullptr, true);
}

pair<Inode *, string> Dir::nameiparent(const std::string &path) {
    string child_name;
    return make_pair(namex(path, &child_name, false), child_name);
}