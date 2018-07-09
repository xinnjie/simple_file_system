//
// Created by capitalg on 2018/7/8.
//
#include <iostream>

#include "rm.h"
using namespace std;

rm::rm(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int rm::run_cmd(std::vector<std::string> args) {
    if (args.size() < 1) {
        cerr << "rm: to few arguments" << endl;
        return -1;
    }
    if (args.size() > 1) {
        cerr << "rm: to many arguments" << endl;
        return -1;
    }
    string path = args[0];
    if (sysfile.unlink(path) == -1) {
        cerr << "rm: " << path << " is not accessible" << endl;
        return -1;
    }
    return 0;
}
