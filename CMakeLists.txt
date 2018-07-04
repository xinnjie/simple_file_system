cmake_minimum_required(VERSION 3.10)
project(file_system)

set(CMAKE_CXX_STANDARD 14)

include_directories(file_system)


add_subdirectory(file_system)
add_subdirectory(file_system_tests)


SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
