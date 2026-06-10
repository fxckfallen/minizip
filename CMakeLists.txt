cmake_minimum_required(VERSION 3.10)

project(MiniZip CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(app main.cpp Archiver.cpp)