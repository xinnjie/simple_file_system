//
// Created by capitalg on 2018/7/8.
//

#ifndef FILE_SYSTEM_SHELL_H
#define FILE_SYSTEM_SHELL_H
#include <memory>

#include <IDEio.h>
#include <Bcache.h>
#include <Icache.h>
#include <Dir.h>
#include <SysFile.h>
#include <map>
#include "abstract_cmd.h"

class Shell {


    std::string fs_name;

    std::map<std::string, std::unique_ptr<abstract_cmd>> commands;
public:
//    static std::string FILE_NAME;

    explicit Shell(const std::string &fs_name);

    void run_cmd(const std::string &cmds);

    void register_cmd(const std::string &command_name, abstract_cmd *cmd);

    void deregister_cmd(const std::string &command_name);
    /**
     * 方便的 split 来自https://stackoverflow.com/a/9437426/7609067
     * @param input
     * @param regex
     * @return
     */
    static std::vector<std::string> split(const std::string &input, const std::string &regex);

    /**
     * 将输入命令 token 化，将字符串以空格为间隔分开（除了引号内的空格）
     * 比较特殊的规则有： 双引号内的内容视为一个 token，
     *
     * @param command_str
     * @return
     */
    static std::vector<std::string> tokenize(const std::string &command_str);


    std::unique_ptr<IDEio> ideio_p;
    std::unique_ptr<Bcache> bcache_p;
    std::unique_ptr<Icache> icache_p;
    std::unique_ptr<Dir> dir_p;
    std::unique_ptr<SysFile> sysfile_p;
    std::unique_ptr<Proc> cur_proc_p;
    std::unique_ptr<Ftable> ftable_p;
};


#endif //FILE_SYSTEM_SHELL_H
