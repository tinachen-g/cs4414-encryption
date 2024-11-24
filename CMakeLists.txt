cmake_minimum_required(VERSION 3.5)
project(bignum)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)

add_library(other_files
  bignum.hpp
  bignum.cpp
)

target_include_directories(other_files PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(bignum main.cpp)

target_link_libraries(bignum PRIVATE other_files)