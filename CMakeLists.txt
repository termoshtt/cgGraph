cmake_minimum_required(VERSION 2.8)

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
