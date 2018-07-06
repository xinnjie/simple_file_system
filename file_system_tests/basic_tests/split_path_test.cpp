//
// Created by capitalg on 2018/7/6.
//

#include <util/split_path.h>
#include "gtest/gtest.h"

using namespace std;

/*
 * "a/bb/c" -> <"a", "bb/c">
 * "a" -> <"a", "">
 * ""-> <"", "">
 */
TEST(split_path_test, basic1) {
    string path = "a/bb/c";
    auto p = split_path(path);
    EXPECT_EQ(p.first, "a");
    EXPECT_EQ(p.second, "bb/c");
}

TEST(split_path_test, basic2) {
    string path = "a";
    auto p = split_path(path);
    EXPECT_EQ(p.first, "a");
    EXPECT_EQ(p.second, "");
}

TEST(split_path_test, basic3) {
    string path = "";
    auto p = split_path(path);
    EXPECT_EQ(p.first, "");
    EXPECT_EQ(p.second, "");
}
