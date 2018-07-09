//
// Created by capitalg on 2018/7/8.
//
#include <iostream>
#include "ls.h"

using namespace std;

ls::ls(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int ls::run_cmd(std::vector<std::string> args) {
    if (args.size() >= 1) {
        cerr << "ls: to many arguments" << endl;
        return -1;
    }
    auto dir_entries = dir.dirls(cur_proc.getCwdi());
    for (auto &dir_entry : dir_entries) {
        cout << dir_entry.name <<  "  " << dir_entry.inum << endl;
    }
    return 0;
}
