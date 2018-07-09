//
// Created by capitalg on 2018/7/8.
//

#include "abstract_cmd.h"

abstract_cmd::abstract_cmd(IDEio &ideio, Bcache &bcache, Icache &icache, Dir &dir, SysFile &sysfile, Proc &cur_proc,
                           Ftable &ftable) : ideio(ideio), bcache(bcache), icache(icache), dir(dir), sysfile(sysfile),
                                             cur_proc(cur_proc), ftable(ftable) {}
