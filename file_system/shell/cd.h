//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_CD_H
#define FILE_SYSTEM_CD_H


#include "abstract_cmd.h"

class cd : public   abstract_cmd{
public:
    cd(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_CD_H
