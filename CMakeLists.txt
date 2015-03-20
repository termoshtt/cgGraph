cmake_minimum_required(VERSION 2.8)

project(cgGraph)

set(CMAKE_BUILD_TYPE Release)
add_definitions("-Wall -std=c++11")
include_directories(.)

add_custom_command(
  OUTPUT cgGraph.pb.cc
  DEPENDS cgGraph.proto
  COMMAND "protoc" "cgGraph.proto" "--cpp_out=." "--python_out=."
)

add_library(cgGraph SHARED
  Cover.cpp
  SimpleCover.cpp
  Timeline.cpp
  TransitionMap.cpp
  algorithm.cpp
  cgGraph.pb.cc
)
target_link_libraries(cgGraph protobuf)

install(TARGETS cgGraph
  EXPORT cgGraphConfig
  INCLUDES DESTINATION include
  LIBRARY DESTINATION lib
)
install(FILES
  Cover.hpp
  SimpleCover.hpp
  Timeline.hpp
  TransitionMap.hpp
  algorithm.hpp
  exception.hpp
  DESTINATION include/cgGraph
)
install(FILES
  cgGraph.py
  cgGraph_pb2.py
  DESTINATION lib
)
install(EXPORT cgGraphConfig
  DESTINATION cmake
)

add_custom_target(document COMMAND "doxygen" "doc/Doxyfile")
install(FILES doc/searchdata.xml DESTINATION doc)
install(DIRECTORY doc/html DESTINATION doc)

enable_testing()
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_subdirectory(test)
add_subdirectory(sample)
