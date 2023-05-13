cmake_minimum_required(VERSION 3.16)
project(GrabCut)

set(CMAKE_CXX_STANDARD 17)

#OpenCV
set(OpenCV_DIR "D:\\DeveloperTools\\opencv-4.7.0-windows\\opencv\\mingw_build\\install")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(./max_flow)

add_executable(GrabCut main.cpp)

add_executable(OpenCVTest OpenCVTest.cpp)
target_link_libraries(OpenCVTest ${OpenCV_LIBS})



