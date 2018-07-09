//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include "touch.h"

using namespace std;

touch::touch(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {
}

int touch::run_cmd(std::vector<std::string> args) {
    if (args.size()  !=  1) {
        cerr << "touch: not suitable arguments, usage: touch example.txt" << endl;
        return -1;
    }
    string file_path = args[0];
    int fd = sysfile.open(file_path, open_option::CREATE);
    if (fd == -1) {
        cerr << "touch: create error" << endl;
        return  -1;
    }
    if (sysfile.close(fd) == -1) {
        cerr << "touch: close error" << endl;
        return -1;
    }
    return 0;
}
