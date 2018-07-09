//
// Created by capitalg on 2018/7/2.
//

#ifndef OS_BIG_LAB_PARAM_H
#define OS_BIG_LAB_PARAM_H
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       1000  // size of file system in blocks

//fs
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)
#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size
#define DIRSIZ 14
#define SUPERBLOCK_INDEX 1



#define SIZE_DINODE 64
#define IPB           (BSIZE / SIZE_DINODE)
#define NINODES 200
#define NBITMAP  (FSSIZE/(BSIZE*8) + 1)
#define NINODEBLOCKS (NINODES / IPB + 1)
// Bitmap bits per block  由于每个磁盘块大小为512B 因此每个磁盘块能标记512*8个磁盘块的空闲情况
#define BPB (BSIZE*8)


#define T_DIR  1   // Directory
#define T_FILE 2   // File


#define DIR_NAME_SZ 12

//#define FS_NAME "fs.all"


// 读写权限, 执行不使用
#define prl_x 0x1
#define prl_w 0x2
#define prl_r 0x4
#define prl_default 0x4;
#define prl_owner ox6

#endif //OS_BIG_LAB_PARAM_H
