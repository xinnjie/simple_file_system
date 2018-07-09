//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_ABSTRACT_CMD_H
#define FILE_SYSTEM_ABSTRACT_CMD_H


#include <Bcache.h>
#include <Icache.h>
#include <Dir.h>
#include <SysFile.h>

class abstract_cmd {
protected:
    IDEio &ideio;
    Bcache &bcache;
    Icache &icache;
    Dir &dir;
    SysFile &sysfile;
    Proc &cur_proc;
    Ftable &ftable;

public:
//    static std::string CMD_NAME = "abstract";
    abstract_cmd(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc,
                 Ftable &ftable);

    abstract_cmd() = delete;

    /**
     * 成功返回0， 失败返回-1
     * @param args
     * @return
     */
    virtual int run_cmd(std::vector<std::string> args) = 0;


//    unique_ptr<IDEio> ideio_p;
//    unique_ptr<Bcache> bcache_p;
//    unique_ptr<Icache> icache_p;
//    unique_ptr<Dir> dir_p;
//    unique_ptr<SysFile> sysfile_p;
//    unique_ptr<Proc> cur_proc_p;
//    unique_ptr<Ftable> ftable_p;
};


#endif //FILE_SYSTEM_ABSTRACT_CMD_H
