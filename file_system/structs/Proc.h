//
// Created by capitalg on 2018/7/7.
//

#ifndef FILE_SYSTEM_PROC_H
#define FILE_SYSTEM_PROC_H

#include <vector>
#include <Icache.h>
#include "Inode.h"
#include "File.h"
#include "User.h"


/**
 * 并不是完整功能的 PCB，只包含了关于文件系统的功能
 */
class Proc {
private:
    Inode *cwdi;
    Icache &icache;
public:
    std::string cwd;
    std::vector<File*> open_files;
    User cur_user;


    /**
     * 最多同时打开 NOFILE 个文件
     */
     explicit Proc(Inode &pathi, const std::string &path, Icache &icache, User &cur_user)
            : open_files(NOFILE), icache(icache), cwd(path), cwdi(
            nullptr), cur_user(cur_user) {
          setCwdi(pathi);
     }

    Inode &getCwdi() const {
         return *cwdi;
    }

    void setCwdi(Inode &cwdi) {
         if (this->cwdi != nullptr) {
              icache.iput(*this->cwdi);
         }
          this->cwdi = &icache.idup(cwdi);
    }
};

#endif //FILE_SYSTEM_PROC_H
