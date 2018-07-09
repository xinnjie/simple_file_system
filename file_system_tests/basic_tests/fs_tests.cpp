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
#include "structs/Dinode.h"



using namespace std;
class fs_test : public ::testing::Test {


protected:
    void SetUp() override {
        string test_name = "fs_test";
        mkfs *mkfs0 = new mkfs(test_name, true);
        mkfs0->make_file_system();
        delete mkfs0;

        ideio_p = make_unique<IDEio>(test_name, FSSIZE);
        bcache_p = make_unique<Bcache>(*ideio_p);



        icache_p = make_unique<Icache>(*bcache_p);
        Inode &root = icache_p->iget(ROOTDEV, ROOTINO);
        User user(0, "root", "root");
        cur_proc_p = make_unique<Proc>(root, "/", *icache_p, user);
        dir_p = make_unique<Dir>(*icache_p, *cur_proc_p);
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
    Dir &dir = *dir_p;
    // 根目录存在 ..  .
    auto dir_entries = dir.dirls(cur_proc.getCwdi());
    EXPECT_EQ(dir_entries.size(), 2);
    EXPECT_STREQ(dir_entries[0].name, ".");
    EXPECT_EQ(dir_entries[0].inum, 1);
    EXPECT_STREQ(dir_entries[1].name, "..");
    EXPECT_EQ(dir_entries[1].inum, 1);



    // 创建目录：绝对路径
    EXPECT_EQ(sysfile.mkdir("/apple"), 0);
    // 创建目录不能影响父目录中其它目录项
    auto dir_entrie_root = dir.dirls(cur_proc.getCwdi());
    EXPECT_EQ(dir_entrie_root.size(), 3);
    EXPECT_STREQ(dir_entrie_root[0].name, ".");
    EXPECT_EQ(dir_entrie_root[0].inum, 1);
    EXPECT_STREQ(dir_entrie_root[1].name, "..");
    EXPECT_EQ(dir_entrie_root[1].inum, 1);
    int dir_inum_apple = dir_entrie_root[2].inum;

    Inode &apple_inode = icache.iget(1, dir_inum_apple);
    EXPECT_EQ(apple_inode.size, sizeof(DirEntry)*2);
    icache.iput(apple_inode);

    EXPECT_EQ(sysfile.chdir("/apple"), 0);
    EXPECT_EQ(cur_proc.cwd, "/apple");

    auto dir_entries_apple0 = dir.dirls(cur_proc.getCwdi());
    EXPECT_EQ(dir_entries_apple0.size(), 2);
    EXPECT_STREQ(dir_entries_apple0[0].name, "..");
    EXPECT_EQ(dir_entries_apple0[0].inum, 1);
    EXPECT_STREQ(dir_entries_apple0[1].name, ".");
    EXPECT_EQ(dir_entries_apple0[1].inum, 2);

    // 创建目录：相对路径
    EXPECT_EQ(sysfile.mkdir("pear"), 0);
    Inode &apple_inode2 = icache.iget(1, dir_inum_apple);
    EXPECT_EQ(apple_inode.size, sizeof(DirEntry)*3);
    icache.iput(apple_inode);


    auto dir_entries_apple1 = dir.dirls(cur_proc.getCwdi());
    EXPECT_EQ(dir_entries_apple1.size(), 3);
    EXPECT_STREQ(dir_entries_apple1[0].name, "..");
    EXPECT_EQ(dir_entries_apple1[0].inum, 1);
    EXPECT_STREQ(dir_entries_apple1[1].name, ".");
    EXPECT_EQ(dir_entries_apple1[1].inum, 2);

    EXPECT_EQ(sysfile.chdir("pear"), 0);
    EXPECT_EQ(cur_proc.cwd, "/apple/pear");

    auto dir_entries_pear = dir.dirls(cur_proc.getCwdi());
    EXPECT_EQ(dir_entries_pear.size(), 2);
    EXPECT_STREQ(dir_entries_pear[0].name, "..");
    EXPECT_EQ(dir_entries_pear[0].inum, dir_inum_apple);
    EXPECT_STREQ(dir_entries_pear[1].name, ".");
    EXPECT_EQ(dir_entries_pear[1].inum, 3);
    int dir_inum_pear = dir_entries_pear[1].inum;

    EXPECT_EQ(sysfile.chdir(".."), 0);
    EXPECT_EQ(cur_proc.cwd, "/apple");

    auto dir_entries_apple3 = dir.dirls(cur_proc.getCwdi());

    Inode &apple_inode3 = icache.iget(1, dir_inum_apple);
    EXPECT_EQ(apple_inode.size, sizeof(DirEntry)*3);
    icache.iput(apple_inode);

    EXPECT_EQ(dir_entries_apple3.size(), 3);
    EXPECT_STREQ(dir_entries_apple3[0].name, "..");
    EXPECT_EQ(dir_entries_apple3[0].inum, 1);
    EXPECT_STREQ(dir_entries_apple3[1].name, ".");
    EXPECT_EQ(dir_entries_apple3[1].inum, dir_inum_apple);
    EXPECT_STREQ(dir_entries_apple3[2].name, "pear");
    EXPECT_EQ(dir_entries_apple3[2].inum, dir_inum_pear);

    EXPECT_EQ(sysfile.chdir("/"), 0);
    EXPECT_EQ(cur_proc.cwd, "/");

}


TEST_F(fs_test, file_operation_test) {
    SysFile &sysfile = *sysfile_p;
    int fd = sysfile.open("helo.txt", open_option::CREATE);
    EXPECT_NE(fd, -1);
    EXPECT_EQ(sysfile.close(fd), 0);
    fd = sysfile.open("helo.txt", open_option::WRITE_ONLY);
    char *content = "hello, world!";
    int n = sysfile.write(fd, content, strlen(content) + 1);
    EXPECT_EQ(n, strlen(content) + 1);

    int read_fd = sysfile.open("helo.txt", open_option::READ_ONLY);
    char buffer[128];
    n = sysfile.read(read_fd, buffer, 128);
    EXPECT_NE(n, -1);
    EXPECT_STREQ(buffer, "hello, world!");
}
