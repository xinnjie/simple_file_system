//
// Created by capitalg on 2018/7/9.
//

#ifndef FILE_SYSTEM_TOUCH_H
#define FILE_SYSTEM_TOUCH_H


#include "abstract_cmd.h"

class touch: public abstract_cmd {
public:
    touch(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_TOUCH_H
