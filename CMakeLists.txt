cmake_minimum_required(VERSION 3.16)
project(GrabCut)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(./max_flow)

add_executable(GrabCut main.cpp)