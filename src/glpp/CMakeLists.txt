find_package(fmt REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(opengl REQUIRED)

add_library(glpp_core)
add_library(glpp::core ALIAS glpp_core)

target_compile_features(glpp_core PUBLIC cxx_std_20)
target_include_directories(
  glpp_core

  PUBLIC
  "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
  glpp_core

  PROPERTIES
  VERSION "${CMAKE_PROJECT_VERSION}"
  SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}"
)
target_sources(
  glpp_core

  PUBLIC
  bit_enum.hpp
  blend.hpp
  buffer.hpp
  depth.hpp
  draw.hpp
  error.hpp
  framebuffer.hpp
  gl.hpp
  id.hpp
  load_shader.hpp
  primitive_types.hpp
  scoped_bind.hpp
  shader.hpp
  shader_program.hpp
  texture.hpp
  traits.hpp
  uniform.hpp
  value_ptr.hpp
  vertex_array.hpp

  PRIVATE
  bit_enum.cpp
  blend.cpp
  buffer.cpp
  depth.cpp
  draw.cpp
  error.cpp
  framebuffer.cpp
  gl.cpp
  id.cpp
  load_shader.cpp
  primitive_types.cpp
  scoped_bind.cpp
  shader.cpp
  shader_program.cpp
  texture.cpp
  traits.cpp
  uniform.cpp
  value_ptr.cpp
  vertex_array.cpp
)
target_link_libraries(
  glpp_core

  PUBLIC
  fmt::fmt
  glad::glad
  glm::glm
  opengl::opengl
  magic_enum::magic_enum
  Microsoft.GSL::GSL
)

if(BUILD_CONFIG)
  add_subdirectory(config)
endif()

if(BUILD_GLFW)
  add_subdirectory(glfw)

  if(BUILD_IMGUI)
    add_subdirectory(imgui)
  endif()
endif()
