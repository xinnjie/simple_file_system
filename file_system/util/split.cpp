//
// Created by capitalg on 2018/7/9.
//

#include <regex>
#include "split.h"

std::vector<std::string> split(const std::string &input, const std::string &regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}