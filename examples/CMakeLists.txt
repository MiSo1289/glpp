if(BUILD_GLFW)
  find_package(lyra REQUIRED)

  add_executable(glfw_input_example)
  target_compile_features(glfw_input_example PRIVATE cxx_std_17)
  target_sources(glfw_input_example PRIVATE glfw_input_example.cpp)
  target_link_libraries(
    glfw_input_example
    PRIVATE
    lyra::lyra
    glpp::glfw
  )

  if(BUILD_CONFIG)
    add_executable(draw_example)
    target_compile_features(draw_example PRIVATE cxx_std_17)
    target_sources(draw_example PRIVATE draw_example.cpp)
    target_link_libraries(
      draw_example

      PRIVATE
      glpp::core
      glpp::config
      glpp::glfw
    )
  endif()

  if(BUILD_IMGUI)
    add_executable(imgui_example)
    target_compile_features(imgui_example PRIVATE cxx_std_17)
    target_sources(imgui_example PRIVATE imgui_example.cpp)
    target_link_libraries(
      imgui_example

      PRIVATE
      glpp::core
      glpp::glfw
      glpp::imgui
    )
  endif()
endif()
