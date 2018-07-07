//
// Created by capitalg on 2018/7/6.
//

#ifndef FILE_SYSTEM_FTABLE_H
#define FILE_SYSTEM_FTABLE_H


#include <structs/File.h>
#include <vector>
#include <structs/FileStat.h>
#include "Icache.h"

class Ftable {
std::vector<File> ftable;
Icache &icache;

public:
    explicit Ftable(Icache &cache);

    File * filealloc();

    /**
     * 对 file 增加计数
     * @param file
     * @return
     */
    File &filedup(File &file);

    void fileclose(File &file);

    int fileread(File &file, char *dest, int n);

    int filewrite(File &file, char *src, int n);

    FileStat stat(const File &file);

};


#endif //FILE_SYSTEM_FTABLE_H
