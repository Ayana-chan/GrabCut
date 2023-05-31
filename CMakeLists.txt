cmake_minimum_required(VERSION 3.16)
project(GrabCut)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(GrabCut main.cpp)
target_link_libraries(GrabCut GrabCutter)
add_subdirectory(src)

add_executable(tempTest tempTest.cpp)
