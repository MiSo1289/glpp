add_executable(glfw_input_example)
target_compile_features(glfw_input_example PRIVATE cxx_std_17)
target_sources(glfw_input_example PRIVATE glfw_input_example.cpp)
target_link_libraries(glfw_input_example PRIVATE glpp::glpp_glfw)

add_executable(draw_example)
target_compile_features(draw_example PRIVATE cxx_std_17)
target_sources(draw_example PRIVATE draw_example.cpp)
target_link_libraries(
	draw_example 
	PRIVATE 
		glpp::glpp
		glpp::glpp_config
		glpp::glpp_glfw
)
