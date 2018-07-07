//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_FILESTAT_H
#define FILE_SYSTEM_FILESTAT_H

#endif //FILE_SYSTEM_FILESTAT_H


struct FileStat {
    short type;  // Type of file
    int dev;     // File system's disk device
    unsigned int inum;    // Inode number
    short nlink; // Number of links to file
    unsigned int size;   // Size of file in bytes
};