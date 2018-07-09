//
// Created by capitalg on 2018/7/8.
//
#include <iostream>
#include <shell/mkfs.h>
#include <shell/cd.h>
#include <shell/pwd.h>
#include <shell/mkdir.h>
#include <shell/ls.h>
#include <shell/rm.h>
#include <shell/ln.h>
#include <shell/cat.h>
#include <shell/touch.h>
#include <util/file_exist.h>


#include "shell/Shell.h"

using namespace std;

string fs_name = "fs.disk";


void register_commands(Shell &shell) {
    IDEio &ideio = *shell.ideio_p;
    Bcache &bcache =  *shell.bcache_p;
    Icache &icache =  *shell.icache_p;
    Dir &dir = *shell.dir_p;
    SysFile &sysfile = *shell.sysfile_p;
    Proc &cur_proc = *shell.cur_proc_p;
    Ftable &ftable = *shell.ftable_p;
    

    shell.register_cmd("cd", new cd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("pwd", new pwd(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("mkdir", new mkdir(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("ls", new ls(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("rm", new rm(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("ln", new ln(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("cat", new cat(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));
    shell.register_cmd("touch", new touch(ideio, bcache, icache, dir, sysfile, cur_proc, ftable));



}
void format() {
    cout << "init file system" << endl;
    mkfs *mkfs0 = new mkfs(fs_name, true);
    mkfs0->make_file_system();
    delete mkfs0;
    cout << "init file system completed" << endl;
}
Shell make_shell() {
    Shell shell(fs_name);
    cout << "runnng file system shell" << endl;

    cout << "registering commands" << endl;
    register_commands(shell);
    cout << "registering commands completed" << endl;
    return shell;
}


int main() {
//    if (!file_exist(fs_name)) {
//        format();
//    }
    format();
    Shell shell = make_shell();
    string cmds;
    cout << "$ " ;
    while (getline(cin, cmds)) {
        if (cmds == "format") { // special command
            format();
            shell = make_shell();
            cout << "$ " ;
            continue;
        }
        shell.run_cmd(cmds);
        cout << "$ " ;
    }

//    auto splits = shell.split("hello world apple pear ", " ");
    return 0;
}