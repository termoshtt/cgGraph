cmake_minimum_required(VERSION 2.8)

project(cgGraph)

set(CMAKE_BUILD_TYPE Release)
add_definitions("-Wall -std=c++11")

add_custom_command(
  OUTPUT cgGraph.pb.cc
  DEPENDS cgGraph.proto
  COMMAND "protoc" "cgGraph.proto" "--cpp_out=." "--python_out=."
)

add_library(cgGraph STATIC
  SimpleCover.cpp
  Timeline.cpp
  TransitionMap.cpp
  cgGraph.pb.cc
)
target_link_libraries(cgGraph protobuf)

add_custom_target(
  document_html
  COMMAND "doxygen" "doc/Doxyfile"
)

enable_testing()
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)
add_subdirectory(test)
