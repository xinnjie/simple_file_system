# simple file system
这是一个为学习文件系统而写的文件系统，复刻自xv6的文件系统部分。为了既能能方便使用OOP的代码封装方式，又能方便地使用字节和位操作（这些低级操作经常出现在磁盘缓冲层中），我选用了C++作为开发语言。

和xv6区别很大的是，这个文件系统是跑在用户层的，而非用户态，能方便得编译运行。

麻雀虽小，五脏俱全。这个文件系统完整得复写了一个文件系统该有的所有功能：磁盘缓冲以及替换策略、路径搜索、（模拟的）文件读写系统调用、权限控制...并且在最外层包装了shell，支持完整的文件、目录的增删查改功能，支持链接


# 说明
与xv6相比，我去掉了日志模块已经多进程的支持，增加了一个简单的权限控制的功能。

# 运行
cd file_system && make run_shell