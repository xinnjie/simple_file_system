//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include "ln.h"

using namespace std;

ln::ln(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int ln::run_cmd(std::vector<std::string> args) {
    if (args.size() < 2) {
        cerr << "ln: to few arguments, usage: ln new_linked_name old_linked_name" << endl;
        return -1;
    }
    if (args.size() > 2) {
        cerr << "ln: to many arguments, usage: ln new_linked_name old_linked_name" << endl;
        return -1;
    }
    string new_file_name = args[0];
    string old_file_name = args[1];
    if (sysfile.link(old_file_name, new_file_name) == -1) {
        cerr << "ln: " << old_file_name << " or " <<  new_file_name << " is not accessible" << endl;
        return -1;
    }
    return 0;}
