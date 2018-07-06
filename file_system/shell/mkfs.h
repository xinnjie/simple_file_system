//
// Created by capitalg on 2018/7/5.
//

#ifndef FILE_SYSTEM_MKFS_H
#define FILE_SYSTEM_MKFS_H

#include <memory>

#include <IDEio.h>
#include <Bcache.h>
#include <Icache.h>

class mkfs {
    std::unique_ptr<IDEio> ideio_p;
    std::unique_ptr<Bcache> bcache_p;
    std::unique_ptr<Icache> icache_p;

    SuperBlock superBlock;


    void init_superblock();

    /**
     * 将 metablock 在初始情况下都标记为已经占用
     * @param from
     * @param to
     */
    void mark_in_use(unsigned int dev, unsigned int from, unsigned int to);
    void mark_in_use(unsigned int dev, unsigned int blockno);

    void insert_root();

public:
    mkfs(std::string fs_name);

    void make_file_system();

    unsigned int nmetablocks;
};


#endif //FILE_SYSTEM_MKFS_H
