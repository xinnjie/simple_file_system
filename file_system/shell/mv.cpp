//
// Created by capitalg on 2018/7/10.
//
#include <iostream>
#include "mv.h"
using namespace std;

mv::mv(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int mv::run_cmd(std::vector<std::string> args) {
    if (args.size() != 2) {
        cerr << "mv: usage mv from_file to_file" << endl;
        return -1;
    }
    string from = args[0];
    string to = args[1];
    if (sysfile.link(to, from) == -1) {
        cerr << "mv: " << from << " is not accessible" << endl;
        return -1;
    }

    if (sysfile.unlink(from) == -1) {
        cerr << "mv: " << from << " is not accessible" << endl;
        return -1;
    }



    return 0;
}
