//
// Created by capitalg on 2018/7/4.
//

#include "gtest/gtest.h"
#include "IDEio.h"


TEST(ideio_test, basic) {
    EXPECT_EQ(1,1);
}


TEST(ideio_test, construct_test) {
    IDEio ideio("test.disk", 1000, false);
}


/*
 * 保证磁盘正常写入
 */
TEST(ideio_test, read_write) {
    IDEio ideio("test.disk", 1000, false);
    Buf buf;
//    memset(buf.data, 5, BSIZE);
    for (int i = 0; i < 50; ++i) {
        *(buf.data+i) = i;
    }
    buf.dev = 1;
    buf.blockno = 3;
    ideio.write(buf);


    Buf readBuf;
    readBuf.dev = 1;
    readBuf.blockno = 3;

    ideio.read(readBuf);
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(*(readBuf.data+i), i);
    }

}

TEST(fwrite_test, fwrite) {

    FILE *file = fopen("hello.test", "w+");
    char *buffer = (char *) calloc(BSIZE, 1000);
    long rest_to_alloc_blocks = 1000;
    long i = 0;
    while (rest_to_alloc_blocks != 0) {
        long written = fwrite(buffer, BSIZE, rest_to_alloc_blocks, file);
        i += BSIZE * written;
        rest_to_alloc_blocks -= written;
    }
    free(buffer);

    fclose(file);


    FILE *reopen = fopen("hello.test", "a+");
    long written = ftell(reopen);
    EXPECT_EQ(written/BSIZE, 1000);

}
