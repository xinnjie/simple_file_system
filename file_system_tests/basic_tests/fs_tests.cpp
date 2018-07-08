//
// Created by capitalg on 2018/7/8.
//



#include <IDEio.h>
#include <Bcache.h>
#include <cstring>
#include <Icache.h>
#include <shell/mkfs.h>
#include <SysFile.h>
#include "gtest/gtest.h"
#include "structs/Buf.h"


using namespace std;
class fs_test : public ::testing::Test {


protected:
    void SetUp() override {
        string test_name = "fs_test";
        mkfs *mkfs0 = new mkfs(test_name);
        mkfs0->make_file_system();
        delete mkfs0;

        ideio_p = make_unique<IDEio>(test_name, FSSIZE);
        bcache_p = make_unique<Bcache>(*ideio_p);



        icache_p = make_unique<Icache>(*bcache_p);
        Inode &root = icache_p->iget(ROOTDEV, ROOTINO);
        dir_p = make_unique<Dir>(*icache_p, &root);
        cur_proc_p = make_unique<Proc>(root, "/");
        ftable_p = make_unique<Ftable>(*icache_p);
        sysfile_p = make_unique<SysFile>(*cur_proc_p, *bcache_p, *icache_p, *ftable_p, *dir_p);
    }


    unique_ptr<IDEio> ideio_p;
    unique_ptr<Bcache> bcache_p;
    unique_ptr<Icache> icache_p;
    unique_ptr<Dir> dir_p;
    unique_ptr<SysFile> sysfile_p;
    unique_ptr<Proc> cur_proc_p;
    unique_ptr<Ftable> ftable_p;



};

TEST_F(fs_test, superblock) {
        Bcache &bcache = *bcache_p;
        SuperBlock sb = bcache.read_superblock(ROOTDEV);
        EXPECT_EQ(sb.size, FSSIZE);
        EXPECT_EQ(sb.inode_start, 2);
}


TEST_F(fs_test, inode_update) {
        Icache &icache = *icache_p;
        Inode &inode = icache.ialloc(1, T_FILE);
        unsigned int inum = inode.inum;

        EXPECT_EQ(inode.type, T_FILE);
        EXPECT_EQ(inode.size, 0);

        inode.type = T_DIR;
        icache.writei(inode, "hello, test", 0, sizeof("hello, test"));
        icache.iupdate(inode);
        icache.iput(inode);
        EXPECT_EQ(inode.ref, 0);

        Inode &another_inode = icache.iget(1, inum);
        EXPECT_EQ(inode.type, T_DIR);
        EXPECT_NE(inode.size, 0);

        Inode read_dinode{};
        read_dinode.dev = inode.dev;
        read_dinode.inum = inode.inum;

        icache.read_dinode(read_dinode);
        EXPECT_EQ(read_dinode.size, inode.size);
        EXPECT_EQ(read_dinode.nlink, inode.nlink);
        EXPECT_EQ(read_dinode.type, inode.type);



}


TEST_F(fs_test, dir_test) {
        SysFile &sysfile = *sysfile_p;
        Proc &cur_proc = *cur_proc_p;
        Icache icache = *icache_p;
        Inode &rooti = *cur_proc.cwdi;

        // 绝对路径
        EXPECT_EQ(sysfile.mkdir("/apple"), 0);
        EXPECT_EQ(sysfile.chdir("/apple"), 0);

        EXPECT_EQ(cur_proc.cwd, "/apple");


}


