//
// Created by capitalg on 2018/7/10.
//

#ifndef FILE_SYSTEM_MV_H
#define FILE_SYSTEM_MV_H


#include "abstract_cmd.h"

class mv : public abstract_cmd{
public:
    mv(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_MV_H
