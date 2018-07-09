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
    User user(0, "root", "root");
    cur_proc_p = make_unique<Proc>(root, "/", *icache_p, user);
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
    auto args = tokenize(cmds);
    if (args.size() < 1) {
        cerr << "shell: please input command" << endl;
        return;
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

std::vector<std::string> Shell::tokenize(const std::string &command_str) {
    vector<unsigned long> quotation_indices;
    vector<string> results;

    quotation_indices.push_back(0);
    for (unsigned long  i = command_str.find('"'); i != string::npos; i = command_str.find('"', i+1)) {
        quotation_indices.push_back(i);
    }
    quotation_indices.push_back(command_str.size());
    if (quotation_indices.size() == 2) {
        return Shell::split(command_str, " ");
    }
    if (quotation_indices.size() % 2 != 0) {
        return  {};
    }
    for (int j = 0; j < quotation_indices.size() - 1; ++j) {
        unsigned long from = quotation_indices[j],
                to = quotation_indices[j+1],
                        len = to - from + 1 ;

        string sub_str = command_str.substr(quotation_indices[j], len);
        if (!(command_str[from] == '"' && command_str[to] == '"')) {
            if (command_str[from] == '"') {
                sub_str = sub_str.substr(1, sub_str.size()-1);
            } else {
                sub_str = sub_str.substr(0, sub_str.size()-1);
            }
            auto split_space = Shell::split(sub_str, " ");
            split_space.erase(std::remove_if(split_space.begin(), split_space.end(), [](string x){
                return x == "" || x == " ";
            }), split_space.end());
            results.insert(results.end(), split_space.begin(), split_space.end());

        } else {
            sub_str = sub_str.substr(1, sub_str.size()-2);
            // 特殊情况fixme "asdsad"{somestring}"asdsd"
            if (sub_str != "" &&  sub_str != " ") {
                results.push_back(sub_str);
            }
        }
    }
    return results;
}


