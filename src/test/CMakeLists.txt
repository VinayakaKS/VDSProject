project(VDSProject_test CXX C)
cmake_minimum_required(VERSION 3.10)


add_executable(VDSProject_test manager_tests.cpp)
target_link_libraries(VDSProject_test Manager)
target_link_libraries(VDSProject_test Reachability)
target_link_libraries(VDSProject_test gtest gtest_main pthread)

