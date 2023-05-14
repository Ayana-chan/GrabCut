include_directories(include)

add_subdirectory(max_flow)
add_subdirectory(cut_alg)
add_subdirectory(ui)
add_subdirectory(util)

add_executable(GrabCut main.cpp GrabCutter.cpp)
target_link_libraries(
        GrabCut
        ${OpenCV_LIBS}
        Util
        CutAlg
        UI
)

