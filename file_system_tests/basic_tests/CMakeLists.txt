project(test)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})


add_executable(runFileSystemTests ideio_tests.cpp bcache_test.cpp inode_tests.cpp split_path_test.cpp fs_tests.cpp)
target_link_libraries(runFileSystemTests gtest gtest_main)
target_link_libraries(runFileSystemTests gmock gmock_main)

target_link_libraries(runFileSystemTests file_system)
