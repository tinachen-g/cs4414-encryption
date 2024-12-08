cmake_minimum_required(VERSION 3.10)
project(bignum VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

add_library(other_files
  bignum.hpp
  bignum.cpp
  operation.hpp
  operation.cpp
  
)

target_include_directories(other_files PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(bignum main.cpp)

target_link_libraries(bignum PRIVATE other_files)