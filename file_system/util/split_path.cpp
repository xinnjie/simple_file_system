//
// Created by capitalg on 2018/7/6.
//

#include <string>
#include "split_path.h"


using namespace std;
/**
 * "a/bb/c" -> <"a", "bb/c">
 * "a" -> <"a", "">
 * ""-> <"", "">
 * @param path
 * @return <rootpath, rest_path>
 */
pair<string, string> split_path(const string &path) {
    auto split_it = find(path.cbegin(), path.cend(), '/');
    if (split_it == path.cend()) {
        return make_pair(path, "");
    }
    return make_pair(string(path.cbegin(), split_it), string(split_it+1, path.cend()));
}