cmake_minimum_required(VERSION 3.13)
project(glpp VERSION 0.2.0)

include(CMakeDependentOption)

option(BUILD_EXAMPLES "Build the examples" ON)
option(BUILD_CONFIG "Build the glpp_config library" ON)
option(BUILD_GLFW "Build the glpp_glfw library" ON)
cmake_dependent_option(
  BUILD_IMGUI "Build the glpp_imgui library" ON
  "BUILD_GLFW" OFF
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

add_subdirectory(src/glpp)

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

set(INSTALL_TARGETS glpp_core)
if(BUILD_CONFIG)
  list(APPEND INSTALL_TARGETS glpp_config)
endif()
if(BUILD_GLFW)
  list(APPEND INSTALL_TARGETS glpp_glfw)
endif()
if(BUILD_IMGUI)
  list(APPEND INSTALL_TARGETS glpp_imgui)
endif()

install(
  TARGETS ${INSTALL_TARGETS}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(
  DIRECTORY src/
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp"
)
