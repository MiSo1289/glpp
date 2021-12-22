find_package(boost REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glpp_glfw)
add_library(glpp::glfw ALIAS glpp_glfw)

target_compile_features(glpp_glfw PUBLIC cxx_std_20)
target_include_directories(
  glpp_glfw

  PUBLIC
  "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
  glpp_glfw

  PROPERTIES
  VERSION "${CMAKE_PROJECT_VERSION}"
  SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}"
)
target_sources(
  glpp_glfw

  PUBLIC
  api_event_context.hpp
  error.hpp
  events.hpp
  glfw.hpp
  keys.hpp
  window.hpp

  PRIVATE
  api_event_context.cpp
  error.cpp
  events.cpp
  glfw.cpp
  keys.cpp
  window.cpp
)
target_link_libraries(
  glpp_glfw

  PUBLIC
  boost::boost
  glfw
  glpp::core
)