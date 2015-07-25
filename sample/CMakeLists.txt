cmake_minimum_required(VERSION 2.8)

macro(sample_executable name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} cgGraph)
endmacro(sample_executable)

sample_executable(Lorenz)
sample_executable(TentMap)
