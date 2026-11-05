cmake_minimum_required(VERSION 3.15)
project(STL_Containers CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(my_stl INTERFACE)
target_include_directories(my_stl INTERFACE include)

enable_testing()
add_subdirectory(tests)