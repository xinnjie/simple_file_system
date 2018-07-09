//
// Created by capitalg on 2018/7/9.
//

#ifndef FILE_SYSTEM_LOGIN_H
#define FILE_SYSTEM_LOGIN_H


#include "abstract_cmd.h"

class login : public abstract_cmd {
public:
    login(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_LOGIN_H
