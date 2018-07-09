//
// Created by capitalg on 2018/7/9.
//

#include "file_exist.h"

bool file_exist(std::string file_name) {
    FILE *f = fopen(file_name.data(), "r");
    if (f != nullptr) {
        fclose(f);
        return true;
    }
    return false;
}