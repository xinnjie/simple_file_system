//
// Created by capitalg on 2018/7/4.
//


#include <IDEio.h>
#include <Bcache.h>
#include <cstring>
#include "gtest/gtest.h"
#include "models/Buf.h"
#include "Util.h"


using namespace std;
class layout_test : public ::testing::Test {


protected:
    void SetUp() override {
        Util::fs_init("layout_test");
        ideio = make_unique<IDEio>("layout_test", FSSIZE);
        bcache = make_unique<Bcache>(*ideio);
    }

    unique_ptr<IDEio> ideio;
    unique_ptr<Bcache> bcache;
};

TEST_F(layout_test, basic_test) {
    SuperBlock superBlock = Util::read_superblock(1, *bcache);
    EXPECT_EQ(superBlock.size, FSSIZE);
}


//TEST_F(layout_test, bcache_test1) {
//    Buf &buf = bcache->bread(1, 10);
//    memset(buf.data, 'a', BSIZE);
//    EXPECT_EQ(buf.data[10], 'a');
//    bcache->bwrite(buf);
//    bcache->brelease(buf);
//
//    Buf &readBuf = bcache->bread(1, 10);
//    EXPECT_EQ(buf.data[10], 'a');
//
//
//}

