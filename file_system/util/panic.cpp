//
// Created by capitalg on 2018/7/5.
//

#include <iostream>
#include "panic.h"
void panic(const std::string &msg) {
    std::cerr << msg << std::endl;
    for (;;) {}
}