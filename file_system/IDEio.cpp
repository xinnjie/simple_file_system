//
// Created by capitalg on 2018/7/3.
//

#include <iostream>
#include "IDEio.h"
using namespace std;

bool file_exist(string file_name) {
    FILE *f = fopen(file_name.data(), "r");
    if (f != NULL) {
        fclose(f);
        return true;
    }
    return false;
}


void IDEio::write(Buf &buf) {
    unsigned int dev = buf.dev,
            blockno = buf.blockno;
    long start_index = blockno * BSIZE;
    fseek(file, start_index, 0);
    assert(ftell(file) == start_index);
    assert(1 == fwrite(buf.data, BSIZE, 1, file));
    assert(ftell(file) == start_index + BSIZE);
    fflush(file);
    buf.flags &= ~B_DIRTY;
}

void IDEio::read(Buf &buf) {
    unsigned int dev = buf.dev,
            blockno = buf.blockno;
    long start_index = blockno * BSIZE;
    fseek(file, start_index, 0);
    assert(1 == fread(buf.data, BSIZE, 1, file));
}

// 申请磁盘空间最简单的方法是打开文件，写入0，直到容量足够。
// https://stackoverflow.com/a/7970461/7609067
IDEio::IDEio(const std::string &fileName, long blocks_num, bool over_write) : file_name(fileName) {
    if (!over_write) {
        if (file_exist(fileName)) {
            file = fopen(file_name.data(), "r+");
        } else {
            file = fopen(file_name.data(), "w+");
        }
    } else {
        file = fopen(file_name.data(), "w+");
    }
    if (file == NULL) {
        perror("can not open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long allocated_bytes = ftell(file);
    long allocated_blocks = allocated_bytes / BSIZE;

    long rest_to_alloc_blocks = blocks_num - allocated_blocks;
    char *buffer = (char *) calloc(BSIZE, rest_to_alloc_blocks);
    if (rest_to_alloc_blocks > 0 &  buffer == NULL) {
        std::cerr << "error allocate mem" << std::endl;
    }

    long i = 0;
    while (rest_to_alloc_blocks > 0) {
        long written = fwrite(buffer, BSIZE, rest_to_alloc_blocks, file);
        i += BSIZE * written;
        rest_to_alloc_blocks -= written;
    }
    free(buffer);
}

IDEio::~IDEio() {
    fclose(file);
}

