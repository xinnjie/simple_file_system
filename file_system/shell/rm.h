//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_RM_H
#define FILE_SYSTEM_RM_H


#include "abstract_cmd.h"

class rm: public abstract_cmd {
public:
    rm(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_RM_H
