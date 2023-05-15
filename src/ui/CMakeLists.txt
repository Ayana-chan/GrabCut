target_sources(GrabCutter
        PUBLIC
        UIController.cpp
        )

add_executable(OpenCVTest OpenCVTest.cpp)
target_link_libraries(OpenCVTest ${OpenCV_LIBS})
