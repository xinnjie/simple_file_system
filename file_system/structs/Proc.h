//
// Created by capitalg on 2018/7/7.
//

#ifndef FILE_SYSTEM_PROC_H
#define FILE_SYSTEM_PROC_H

#include <vector>
#include <Icache.h>
#include "Inode.h"
#include "File.h"


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


    /**
     * 最多同时打开 NOFILE 个文件
     */
     explicit Proc(Inode &pathi, const std::string &path, Icache &icache): open_files(NOFILE),icache(icache),cwd(path), cwdi(
            nullptr) {
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
