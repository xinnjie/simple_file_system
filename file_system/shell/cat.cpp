//
// Created by capitalg on 2018/7/8.
//
#include <iostream>
#include "cat.h"

using namespace std;

int cat::run_cmd(std::vector<std::string> args) {
    if (args.size()  ==  1) {
        string file_path = args[0];
        int fd = sysfile.open(file_path, open_option::READ_ONLY);
        if (fd == -1) {
            cerr << "cat: " << file_path << " is not accessible" << endl;
            return  -1;
        }
        char buffer[512];
        int n = sysfile.read(fd, buffer, 511);
        if (sysfile.close(fd) == -1) {
            cerr << "cat: close error" << endl;
            return -1;
        }
        buffer[n] = '\0';
        cout << buffer << endl;
        return 0;
    }
    if (args.size() == 3) {
        string content = args[0], file_path = args[2];
        if (args[1] != ">") {
            cerr << "cat: usage cat \"hello world!\" > example.txt or \n cat example.txt" << endl;
            return  -1;
        }
        int fd = sysfile.open(file_path, open_option::WRITE_ONLY);
        if (fd == -1) {
            cerr << "cat: " << file_path << " is not accessible" << endl;
            return  -1;
        }


        int n = sysfile.write(fd, content.c_str(), content.size() + 1);
        if (n != content.size() + 1) {
            cerr << "cat: " << n << "written, left " << content.size() + 1 << " to write" << endl;
        }

        if (sysfile.close(fd) == -1) {
            cerr << "cat: close error" << endl;
            return -1;
        }
        return 0;

    }
    cerr << "cat: to few arguments, usage cat \"hello world!\" > example.txt or \n cat example.txt" << endl;
    return -1;
}

cat::cat(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc, Ftable &ftable)
        : abstract_cmd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable) {

}
