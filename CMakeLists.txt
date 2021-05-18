cmake_minimum_required(VERSION 3.17)
project(compare)

set(CMAKE_CXX_STANDARD 11)
add_executable(compare main.cpp)
target_link_libraries(compare pthread)
