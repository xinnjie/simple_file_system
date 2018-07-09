//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include "pwd.h"

using namespace std;
int pwd::run_cmd(std::vector<std::string> args) {
    if (args.size() > 0) {
        cerr << "pwd: to many arguments" << endl;
        return -1;
    }
    cout << cur_proc.cwd << endl;
    return 0;
}

pwd::pwd(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}
