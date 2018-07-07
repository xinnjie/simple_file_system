//
// Created by capitalg on 2018/7/6.
//

#include <util/panic.h>
#include "Ftable.h"

Ftable::Ftable(Icache &cache): ftable(NFILE), icache(cache) {}

File &Ftable::filedup(File &file) {
    file.ref++;
    return file;
}

File * Ftable::filealloc(void) {
    for (int i = 0; i < ftable.size(); ++i) {
        if (ftable[i].ref == 0) {
            ftable[i].ref = 1;
            return &ftable[i];
        }
    }
    return nullptr;
}

void Ftable::fileclose(File &file) {
    if (file.ref < 1) {
        panic("fileclose: ref count < 1");
    }
    if (--file.ref > 0) {
        return;
    }

    File temp = file;
    file.ref = 0;
    file.type = fd_type::FD_NONE;

    icache.iput(*temp.ip);

}

int Ftable::fileread(File &file, char *dest, int n) {
    if (!file.readable) {
        return -1;
    }
    if (file.type == fd_type::FD_INODE) {
        int read = 0;
        if ((read = icache.readi(*file.ip, dest, file.offset, n)) > 0) {
            file.offset += read;
        }
        return read;
    }
    panic("fileread");
}

int Ftable::filewrite(File &file, char *src, int n) {
    if (!file.writable) {
        return -1;
    }
    if (file.type == fd_type::FD_INODE) {
        // fixme 限制一次最大可读字符数量，否则可能磁盘容量可能不够
        int written = 0;
        if ((written = icache.writei(*file.ip, src, file.offset, n)) > 0) {
            file.offset += written;
        }
        if (written != n) {
            panic("filewrite: failed to write");
        }
        return written;
    }
    panic("filewrite");
}

FileStat Ftable::stat(const File &file) {
    if (file.type == fd_type::FD_INODE) {
        FileStat stat;
        Inode &inode = *file.ip;
        stat.dev = inode.dev;
        stat.inum = inode.inum;
        stat.type = inode.type;
        stat.nlink = inode.nlink;
        stat.size = inode.size;

        return stat;
    }
    panic("stat");
}
