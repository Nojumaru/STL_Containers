cmake_minimum_required(VERSION 3.15)
project(STL_Containers CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(nojo INTERFACE)
target_include_directories(nojo INTERFACE include)

enable_testing()
add_subdirectory(tests)