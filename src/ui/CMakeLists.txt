add_library(
        UI
        UIController.cpp
)

target_link_libraries(UI ${OpenCV_LIBS})

add_executable(OpenCVTest OpenCVTest.cpp)
target_link_libraries(OpenCVTest ${OpenCV_LIBS})