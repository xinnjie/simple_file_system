//
// Created by capitalg on 2018/7/4.
//

#ifndef FILE_SYSTEM_ICACHE_H
#define FILE_SYSTEM_ICACHE_H


#include <vector>
#include <models/Inode.h>
#include "Bcache.h"

class Icache {
private:
    std::vector<Inode> inodes;
    Bcache &bcache;
    SuperBlock superBlock;


    inline unsigned int inode_location(int inum);
    /**
     * 返回特定 inode 第 bn 块的块号, 如果该块还没有被分配，也会进行分配后返回新分配的块号
     * @param inode
     * @param bn
     * @return
     */
    inline unsigned int find_block_location(Inode &inode, unsigned int bn);

    void read_dinode(Inode &inode);
public:
    explicit Icache(Bcache &cache);

    /**
     * 从磁盘中找到一个空闲的i节点，并将该 i 节点类型设置为 type
     * @param dev
     * @param type FD_NONE 表示空闲， FD_INODE 表示文件 fixme 理解问题？
     * @return
     */
    Inode &ialloc(unsigned int dev, short type);

    /**
     * 删除指定的 i 节点, 已经该节点包含的磁盘块
     * **注意** i 节点层不关心该 i节点是文件还是目录（目录涉及到级联删除），会在目录层完成这些功能
     * @param inode
     */
    void idelete(Inode &inode);

    Inode &iget(unsigned int dev, unsigned int inum);

    /**
     * 关闭文件时调用iput 减少 iNode ref 计数
     * @param inode
     */
    void iput(Inode &inode);

    /**
     * 将 inode 信息同步磁盘
     * @param inode
     */
    void iupdate(Inode &inode);


    Inode &idup(Inode &inode);

    /**
     * 假如 n 大于实际可读的内容，会从 off 开始读出剩余所有内容，返回真实读取的字符数
     * 如果 off 大于可读内容，返回-1
     *
     * 正常写入数据时，如果写入后文件大小比原来更大，会调用 iupdate 更新 i 节点信息
     * @param inode
     * @param dest
     * @param off
     * @param n
     * @return 读取的字符数
     */
    int readi(Inode &inode, char* dest, unsigned int off, unsigned int n);

    int writei(Inode &inode, const char *src, unsigned int off, unsigned int n);
};


#endif //FILE_SYSTEM_ICACHE_H
