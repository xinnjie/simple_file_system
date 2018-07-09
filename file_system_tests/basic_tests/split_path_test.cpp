//
// Created by capitalg on 2018/7/6.
//

#include <util/split_path.h>
#include <shell/Shell.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using namespace testing;

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

TEST(split_command, basic1) {
    string command = "cat \"hello , world\" > hello.txt";
    auto splits = Shell::tokenize(command);
    ASSERT_THAT(splits, ElementsAre("cat", "hello , world", ">", "hello.txt"));

}
TEST(split_command, basic2) {
    string command = "cat \"hello , world\" \"he   sfhkjhefe f\" > hello.txt";
    auto splits = Shell::tokenize(command);
    ASSERT_THAT(splits, ElementsAre("cat", "hello , world", "he   sfhkjhefe f" , ">", "hello.txt"));

}
