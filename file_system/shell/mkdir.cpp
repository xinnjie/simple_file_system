//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include "mkdir.h"

using namespace std;
mkdir::mkdir(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int mkdir::run_cmd(std::vector<std::string> args) {
    if (args.size() < 1) {
        cerr << "mkdir: to few arguments" << endl;
        return -1;
    }
    if (args.size() > 1) {
        cerr << "mkdir: to many arguments" << endl;
        return -1;
    }
    string path = args[0];
    if (sysfile.mkdir(path) == -1) {
        cerr << "mkdir: " << path << " is not accessible" << endl;
        return -1;
    }
    return 0;
}
