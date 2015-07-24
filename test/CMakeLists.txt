cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

macro(mytest name)
  set(test_name TEST_${name})
  add_executable(${test_name} ${name}.cpp)
  target_link_libraries(${test_name} cgGraph ${Boost_LIBRARIES} ${ARGN})
  add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
  unset(test_name)
endmacro(mytest)

mytest(Cover)
mytest(timeline)
mytest(transition_map)
mytest(information_function)
