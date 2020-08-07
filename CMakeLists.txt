cmake_minimum_required(VERSION 3.13)
project(Glpp VERSION 0.1.0)

option(USING_CONAN "Set by conan during installation" OFF)
add_subdirectory(src/glpp)

option(BUILD_EXAMPLES "Build the examples" ON)
if (BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
