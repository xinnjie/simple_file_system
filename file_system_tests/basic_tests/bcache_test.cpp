//
// Created by capitalg on 2018/7/4.
//


#include <IDEio.h>
#include <Bcache.h>
#include <cstring>
#include "gtest/gtest.h"
#include "structs/Buf.h"


using namespace std;
class bcache_test : public ::testing::Test {


protected:
    void SetUp() override {
        ideio = make_unique<IDEio>("bcache_test", 200);
        bcache = make_unique<Bcache>(*ideio);
    }


    unique_ptr<IDEio> ideio;
    unique_ptr<Bcache> bcache;
};

TEST_F(bcache_test, basic_test) {
    EXPECT_EQ(0, 0);
    EXPECT_TRUE(ideio != nullptr);
    EXPECT_TRUE(bcache != nullptr);
}


TEST_F(bcache_test, bcache_test1) {
    Buf &buf = bcache->bread(1, 10);
    memset(buf.data, 'a', BSIZE);
    EXPECT_EQ(buf.data[10], 'a');
    bcache->bwrite(buf);
    bcache->brelease(buf);

    Buf &readBuf = bcache->bread(1, 10);
    EXPECT_EQ(buf.data[10], 'a');


}

