//
// Created by capitalg on 2018/7/8.
//
#include <regex>
#include <iostream>
#include "Shell.h"

using namespace std;


Shell::Shell(const std::string &fs_name): fs_name(fs_name) {
//    string test_name = "fs_test";
//    mkfs *mkfs0 = new mkfs(test_name);
//    mkfs0->make_file_system();
//    delete mkfs0;
    ideio_p = make_unique<IDEio>(fs_name, FSSIZE);
    bcache_p = make_unique<Bcache>(*ideio_p);
    icache_p = make_unique<Icache>(*bcache_p);
    Inode &root = icache_p->iget(ROOTDEV, ROOTINO);
    cur_proc_p = make_unique<Proc>(root, "/", *icache_p);
    dir_p = make_unique<Dir>(*icache_p, *cur_proc_p);
    ftable_p = make_unique<Ftable>(*icache_p);
    sysfile_p = make_unique<SysFile>(*cur_proc_p, *bcache_p, *icache_p, *ftable_p, *dir_p);
}

std::vector<std::string> Shell::split(const std::string &input, const std::string &regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}

void Shell::run_cmd(const std::string &cmds) {
    auto args = split(cmds, " ");
    if (args.size() < 1) {
        cerr << "shell: please input command" << endl;
    }
    string command = args[0];
    auto it = commands.find(command);
    if (it == commands.end()) { // command not found
        cerr << "shell: command not found" << endl;
        return;
    }
    auto &command_func = it->second;
    command_func->run_cmd({args.begin()+1, args.end()});

}

void Shell::register_cmd(const std::string &command_name, abstract_cmd *cmd) {
    commands.emplace(command_name, cmd);
}

void Shell::deregister_cmd(const std::string &command_name) {
    commands.erase(command_name);
}


