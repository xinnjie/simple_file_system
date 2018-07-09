//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_CAT_H
#define FILE_SYSTEM_CAT_H


#include "abstract_cmd.h"

class cat: public abstract_cmd {

public:
    cat(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable);

    /**
     * examples:
     * cat example.txt
     * cat "hello world!" > example.txt
     * @param args
     * @return
     */
    int run_cmd(std::vector<std::string> args) override;
};


#endif //FILE_SYSTEM_CAT_H
