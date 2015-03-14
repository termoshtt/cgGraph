cmake_minimum_required(VERSION 2.8)

add_executable(Lorenz Lorenz.cpp)
target_link_libraries(Lorenz cgGraph)
