//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include <util/get_unique_id.h>

#include "useradd.h"
using namespace std;

useradd::useradd(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc,
                 Ftable &ftable) : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int useradd::run_cmd(std::vector<std::string> args) {
    if (args.size() != 2) {
        cerr << "useradd: usage useradd user_name password" << endl;
        return -1;
    }
    string username = args[0],
            userpasswd = args[1];

    int fd = sysfile.open("/etc/.passwd", open_option::READ_ONLY);
    if (fd == -1) {
        cerr << "useradd: open .passwd faild" << endl;
        return -1;
    }
    char content[256];
    int n = sysfile.read(fd, content, 255);
    if (n == -1) {
        cerr << "useradd: read .passwd faild" << endl;
        return -1;
    }
    content[n] = '\0';
    if (sysfile.close(fd) == -1) {
        cerr << "useradd: close error" << endl;
        return -1;
    }
    string userinfo(content);
    userinfo += to_string(get_unique_id()) + " "  + username + " " + userpasswd + "|";

    fd = sysfile.open("/etc/.passwd", open_option::WRITE_ONLY);
    if (fd == -1) {
        cerr << "useradd: open .passwd faild" << endl;
        return -1;
    }
    n = sysfile.write(fd, userinfo.c_str(), userinfo.size());
    if (n != userinfo.size()) {
        cerr << "useradd: " << n << "written, left " << userinfo.size()  << " to write" << endl;
    }

    if (sysfile.close(fd) == -1) {
        cerr << "useradd: close error" << endl;
        return -1;
    }

    string path = "/users/" + username;
    if (sysfile.mkdir(path) == -1) {
        cerr << "useradd: " << path << " is not accessible" << endl;
        return -1;
    }
    return 0;


}
