cmake_minimum_required(VERSION 3.13)
project(Glpp VERSION 0.1)

add_subdirectory(src/glpp)

option(BUILD_EXAMPLES "Build the examples" ON)
if (BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
