//
// Created by capitalg on 2018/7/7.
//
#include <iostream>
#include <util/panic.h>
#include <structs/DirEntry.h>
#include "SysFile.h"


using namespace std;

string path_concate(const string &base_path, const string &relative_path) {
    // fixme cd 时有很多特殊情况 cd ..   cd ../hello   cd .
    if (relative_path == "..") {
        int i = base_path.find_last_of('/');
        string result{base_path.cbegin(), base_path.cbegin() + i};
        if (result.empty()) {
            return "/";
        }
        return result;
    }

    if (*(base_path.end()-1) == '/') {
        return base_path + relative_path;
    }
    return base_path + "/" + relative_path;
}

File *SysFile::argfd(int fd) {
    if (fd < 0 || fd >= NOFILE) {
        return nullptr;
    }

    return cur_proc.open_files[fd];
}

int SysFile::fdalloc(File &f) {
    for (int i = 0; i < NOFILE; ++i) {
        if (cur_proc.open_files[i] == nullptr) {
            cur_proc.open_files[i] = &f;
            return i;
        }
    }
    return -1;
}

int SysFile::dup(int fd) {
    File *f = argfd(fd);
    if (f == nullptr) {
        return -1;
    }
    int new_fd;
    if ((new_fd = fdalloc(*f)) == -1) {
        return -1;
    }
    ftable.filedup(*f);
    return new_fd;
}

int SysFile::read(int fd, char *dest, int n) {
    File *f;
    if ((f = argfd(fd))== nullptr) {
        return -1;
    }
    return ftable.fileread(*f, dest, n);
}

int SysFile::write(int fd, char *src, int n) {
    File *f;
    if ((f = argfd(fd))== nullptr) {
        return -1;
    }
    return ftable.filewrite(*f, src, n);
}

int SysFile::close(int fd) {
    File *f;
    if ((f = argfd(fd))== nullptr) {
        return -1;
    }
    cur_proc.open_files[fd] = nullptr;
    ftable.fileclose(*f);
    return 0;
}

int SysFile::link(const std::string &new_name, const std::string &path) {
    Inode *ip = dir_util.namei(path);
    if (ip == nullptr) {
        return -1;
    }

    if (ip->type == T_DIR) {
        return -1;
    }

    auto temp = dir_util.nameiparent(new_name);
    Inode *parent_ip = temp.first;
    string &child_name = temp.second;

    if (parent_ip == nullptr || child_name.empty()) {
        return -1;
    }

    if (dir_util.dirlink(*parent_ip, child_name, ip->inum) == -1) {
        return -1;
    }
    ip->nlink++;
    icache.iupdate(*ip);
    icache.iput(*ip);
    icache.iput(*parent_ip);
    return 0;
}

int SysFile::unlink(const string &path) {
    Inode *dp, *ip;

    auto temp = dir_util.nameiparent(path);
    string child_name = temp.second;
    if ((dp = temp.first) == nullptr) {
        return -1;
    }
    if (child_name == "." || child_name == "..") {
        icache.iput(*dp);
        return -1;
    }
    auto temp1 = dir_util.dirlookup(*dp, child_name);
    ip = temp1.first;
    int offset = temp1.second;
    if (ip == nullptr) {
        icache.iput(*dp);
        return -1;
    }

    if (ip->nlink < 1) {
        panic("unlink: nlink < 1");
    }

    // 不能对 不为空的目录进行 unlink
    if (ip->type == T_DIR && !isdirempty(*ip)) {
        icache.iput(*dp);
        icache.iput(*ip);
        cerr << "unlink: can only unlink empty dir" << endl;
        return -1;
    }

    DirEntry dirEntry;
    if (icache.writei(*dp, reinterpret_cast<char*>(&dirEntry), offset, sizeof(dirEntry)) != sizeof(dirEntry)) {
        panic("unlink: writei incomplete");
    }

    //  如果是目录，会有..指向父目录，所以删去目录时，父母的计数需要-1
    if (ip->type == T_DIR) {
        dp->nlink--;
        icache.iupdate(*dp);
    }
    icache.iput(*dp);
    ip->nlink--;
    icache.iupdate(*ip);
    return 0;
}

bool SysFile::isdirempty(Inode &dir_inode) {
    DirEntry dirEntry;
    // 跳过头两个 "."  ".."
    for (int off = 2*sizeof(DirEntry); off < dir_inode.size; off+= sizeof(DirEntry)) {
        if (icache.readi(dir_inode, reinterpret_cast<char*>(&dirEntry), off, sizeof(DirEntry)) != sizeof(DirEntry)) {
            panic("isdirempty: readi");
        }
        if (dirEntry.inum != 0) {
            return false;
        }
    }
    return true;
}

Inode *SysFile::create(const std::string &path, short type) {
    Inode *dp, *ip;

    auto parent_and_name = dir_util.nameiparent(path);
    dp = parent_and_name.first;
    string name = parent_and_name.second;
    if (dp == nullptr) {
        return nullptr;
    }
    if ((ip = dir_util.dirlookup(*dp, name).first) != nullptr) {     // 文件已经存在
        if (type == T_FILE && ip->type == T_FILE) {
            return ip;
        }
        // fixme 遇到重名的目录，应该能创建，这里不能
        icache.iput(*dp);
        icache.iput(*ip);
        return nullptr;
    }
    if ((ip = &icache.ialloc(dp->dev, type)) == nullptr) {
        panic("create: ialloc");
    }
    ip->nlink = 1;
    icache.iupdate(*ip);
    if (type == T_DIR) { // 如果是目录，需要创建 . 和..
        dp->nlink++; // .. 导致父目录计数+1
        icache.iupdate(*dp);
        if (dir_util.dirlink(*ip, "..", dp->inum) == -1 || dir_util.dirlink(*ip, ".", ip->inum) == -1) {
            panic("create: dots");
        }
    }
    if (dir_util.dirlink(*dp, name, ip->inum) == -1) { // 在父目录中增加这一项
        panic("create: dirlink");
    }
    icache.iput(*dp);
    return ip;
}

int SysFile::open(const std::string &path, open_option op) {
    Inode *ip;
    File *f;
    int fd = 0;
    if(op == open_option::CREATE) {
        if ((ip = create(path, T_FILE)) == nullptr) {
            return -1;
        }
    }
    if ((ip = dir_util.namei(path)) == nullptr) {
        return -1;
    }
    // 目录不允许写入
    if (ip->type == T_DIR && op != open_option::READ_ONLY) {
        icache.iput(*ip);
        return -1;
    }

    if ((f = ftable.filealloc()) == nullptr || (fd = fdalloc(*f)) == -1) {
        if (f) {
            ftable.fileclose(*f);
        }
        icache.iput(*ip);
        return -1;
    }


    f->type = fd_type::FD_INODE;
    f->ip = ip;
    f->offset = 0;
    f->readable = !(op == open_option::WRITE_ONLY);
    f->readable = (op == open_option::WRITE_ONLY || op == open_option::WRITE_READ);
    return fd;
}

int SysFile::mkdir(const std::string &path) {
    Inode *ip;
    if ((ip = create(path, T_DIR)) == nullptr) {
        return -1;
    }
    icache.iput(*ip);
    return 0;
}

int SysFile::chdir(const std::string &path) {
    Inode *ip;
    if ((ip = dir_util.namei(path)) == nullptr) {
        return -1;
    }
    if (ip->type != T_DIR) {
        icache.iput(*ip);
        return -1;
    }
    cur_proc.setCwdi(*ip);
    if (path[0] == '/') {
        cur_proc.cwd = path;
    } else {
        cur_proc.cwd = path_concate(cur_proc.cwd, path);
    }

    return 0;
}



