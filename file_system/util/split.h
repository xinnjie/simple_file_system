//
// Created by capitalg on 2018/7/9.
//

#ifndef FILE_SYSTEM_SPLIT_H
#define FILE_SYSTEM_SPLIT_H
#include<string>
#include<vector>
/**
    * 方便的 split 来自https://stackoverflow.com/a/9437426/7609067
    * @param input
    * @param regex
    * @return
    */
std::vector<std::string> split(const std::string &input, const std::string &regex);

#endif //FILE_SYSTEM_SPLIT_H
