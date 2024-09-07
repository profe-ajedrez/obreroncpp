cmake_minimum_required(VERSION 3.5.0)
project(obreroncpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_SCAN_FOR_MODULES True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -O0 -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(RUNTIME_OUTPUT_DIRECTORY "build")
set(CMAKE_VERBOSE_MAKEFILE True)

add_library(obreroncpp obreroncpp.cpp)

target_link_libraries(imxcore mysqlcppconn VERSION 1.1.12-4)
