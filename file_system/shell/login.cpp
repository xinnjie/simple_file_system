//
// Created by capitalg on 2018/7/9.
//
#include <iostream>
#include <util/split.h>

#include "login.h"

using namespace std;

login::login(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}

int login::run_cmd(std::vector<std::string> args) {
    if (args.size() != 2) {
        cerr << "login: usage login user_name password" << endl;
        return -1;
    }
    string username = args[0],
            userpasswd = args[1];

    int fd = sysfile.open("/etc/.passwd", open_option::READ_ONLY);
    if (fd == -1) {
        cerr << "login: open .passwd faild" << endl;
        return -1;
    }
    char content[256];
    int n = sysfile.read(fd, content, 255);
    if (n == -1) {
        cerr << "login: read .passwd faild" << endl;
        return -1;
    }
    content[n] = '\0';
    if (sysfile.close(fd) == -1) {
        cerr << "login: close error" << endl;
        return -1;
    }
    string userinfo(content);
    auto userinfos = split(userinfo, "\\|");
    for (auto &user_str : userinfos) {

        auto name_pass = split(user_str, " ");
        assert(name_pass.size() == 3);
        string id_str = name_pass[0];
        string name = name_pass[1];
        string pass = name_pass[2];
        if (name == username) {
            if (pass == userpasswd) {
                cout << "login: switch to uid: " << id_str << " <"  << name << ">" << endl;
                User &cur_user = cur_proc.cur_user;
                cur_user.uid = stoi(id_str);
                cur_user.uname = name;
                cur_user.passwd = pass;
                return 0;
            }
            else {
                cerr << "login: wrong password" << endl;
                return -1;
            }
        }
    }
    cerr << "login: can not find user: " << username << endl;
    return -1;
}
