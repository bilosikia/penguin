cmake_minimum_required(VERSION 3.7)
project(penguin)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp src/universal_io.h)
add_executable(penguin ${SOURCE_FILES})