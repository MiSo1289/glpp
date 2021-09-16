add_library(glpp_config)
add_library(glpp::glpp_config ALIAS glpp_config)

target_compile_features(glpp_config PUBLIC cxx_std_20)
target_include_directories(
  glpp_config

  PUBLIC
  "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
  glpp_config

  PROPERTIES
  VERSION "${CMAKE_PROJECT_VERSION}"
  SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}"
)
target_sources(
  glpp_config

  PUBLIC
  config.hpp
  error.hpp

  PRIVATE
  config.cpp
  error.cpp
)

target_link_libraries(
  glpp_config

  PUBLIC
  CONAN_PKG::nlohmann_json
  glpp::glpp
)
