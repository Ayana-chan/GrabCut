add_library(MaxFlow MaxFlowTest.cpp block.h graph.cpp graph.h instances.inc maxflow.cpp)

target_include_directories(MaxFlow PUBLIC .)

add_executable(MaxFlowTest MaxFlowTest.cpp block.h graph.cpp graph.h instances.inc maxflow.cpp)
