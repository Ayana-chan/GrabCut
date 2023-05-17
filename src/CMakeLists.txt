#OpenCV
set(OpenCV_DIR "D:\\DeveloperTools\\opencv-4.7.0-windows\\opencv\\mingw_build\\install")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(GrabCutter GrabCutter.cpp)
target_include_directories(GrabCutter PUBLIC include)

target_link_libraries(GrabCutter ${OpenCV_LIBS})

add_subdirectory(cut_alg)
add_subdirectory(ui)
add_subdirectory(util)

add_subdirectory(max_flow)
target_link_libraries(GrabCutter MaxFlow)




