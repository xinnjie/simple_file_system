//
// Created by capitalg on 2018/7/5.
//


#include <IDEio.h>
#include <Bcache.h>
#include <cstring>
#include <Icache.h>
#include <Util.h>
#include <shell/mkfs.h>
#include "gtest/gtest.h"
#include "structs/Buf.h"


using namespace std;
class inode_test : public ::testing::Test {


protected:
    void SetUp() override {
        string test_name = "inode_test";
        mkfs *mkfs0 = new mkfs(test_name);
        mkfs0->make_file_system();
        delete mkfs0;

        ideio = make_unique<IDEio>(test_name, FSSIZE);
        bcache = make_unique<Bcache>(*ideio);
        icache = make_unique<Icache>(*bcache);
    }


    unique_ptr<IDEio> ideio;
    unique_ptr<Bcache> bcache;
    unique_ptr<Icache> icache;

};

TEST_F(inode_test, basic_test) {
//    unsigned int zero_blockno= bcache->balloc(1);
//     EXPECT_EQ(zero_blockno, 0);

    Inode &inode = icache->ialloc(1, T_FILE);



    inode.nlink = 1;
    icache->iupdate(inode);

    string test_str = "hello, file system!";

    unsigned int n =  icache->writei(inode, test_str.data(), 0, test_str.size());

    char copy[100];
    EXPECT_EQ(n, icache->readi(inode, copy, 0, n));
    copy[n] = '\0';
    EXPECT_EQ(test_str, copy);


}


