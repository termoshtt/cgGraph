cmake_minimum_required(VERSION 2.8)

project(cgGraph)

set(CMAKE_BUILD_TYPE Release)
add_definitions("-Wall -std=c++11")
include_directories(.)

find_package(Boost COMPONENTS
  unit_test_framework
  system
REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)
enable_testing()

add_custom_command(
  OUTPUT cgGraph.pb.cc cgGraph.pb.h
  DEPENDS cgGraph.proto
  COMMAND "protoc" "cgGraph.proto" "--cpp_out=." "--python_out=."
)

add_library(cgGraph STATIC
  Cover.cpp
  SimpleCover.cpp
  TransitionMap.cpp
  algorithm.cpp
  information_function.cpp
  cgGraph.pb.cc
)
target_link_libraries(cgGraph
  protobuf
  ${Boost_LIBRARIES}
)

install(TARGETS cgGraph
  EXPORT cgGraphConfig
  INCLUDES DESTINATION include
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES
  Cover.hpp
  SimpleCover.hpp
  Timeline.hpp
  TransitionMap.hpp
  algorithm.hpp
  exception.hpp
  cgGraph.pb.h
  DESTINATION include/cgGraph
)
install(FILES
  cgGraph.py
  cgGraph_pb2.py
  DESTINATION lib
)
install(PROGRAMS
  cgGraph_render
  cgGraph_stats
  DESTINATION bin
)
install(EXPORT cgGraphConfig
  DESTINATION cmake
)

add_custom_target(ctags ALL COMMAND "ctags" "-R" "-f" ".tags" "--languages=C++,python" "--exclude='CMake*'")
add_custom_target(document COMMAND "doxygen" "doc/Doxyfile")

add_subdirectory(test)
add_subdirectory(sample)
