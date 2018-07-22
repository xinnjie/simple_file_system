//
// Created by capitalg on 2018/7/10.
//
#include <iostream>

#include "logout.h"
using namespace std;

logout::logout(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int logout::run_cmd(std::vector<std::string> args) {
    if (args.size() != 0) {
        cerr << "logout: to few arguments" << endl;
        return -1;
    }
    cur_proc.cur_user.uname = "";
    cerr << "logout: logout" << endl;
    return 0;
}
