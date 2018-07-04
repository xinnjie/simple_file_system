//
// Created by capitalg on 2018/7/3.
//

#ifndef FILE_SYSTEM_IDEIO_H
#define FILE_SYSTEM_IDEIO_H


#include <string>
#include "Buf.h"

class IDEio {
private:
    std::string file_name;
    std::FILE *file;

public:
    IDEio(const std::string &fileName, long blocks_num);

    virtual ~IDEio();

public:
    void write(Buf &buf);
    void read(Buf &buf);

    fpos_t f_size;
};


#endif //FILE_SYSTEM_IDEIO_H
