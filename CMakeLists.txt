cmake_minimum_required(VERSION 3.13)
project(Glpp VERSION 0.2.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
include(conanbuildinfo)
conan_basic_setup(TARGETS)

add_subdirectory(src/glpp)

option(BUILD_EXAMPLES "Build the examples" ON)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
