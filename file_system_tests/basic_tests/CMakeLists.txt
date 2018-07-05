project(test)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runFileSystemTests ideio_tests.cpp bcache_test.cpp layout_tests.cpp)
target_link_libraries(runFileSystemTests gtest gtest_main)
target_link_libraries(runFileSystemTests file_system)
