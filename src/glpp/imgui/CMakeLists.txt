add_library(glpp_imgui)
add_library(glpp::glpp_imgui ALIAS glpp_imgui)

target_compile_features(
	glpp_imgui
	PUBLIC 
		cxx_std_17
)
target_include_directories(
	glpp_imgui 
	PUBLIC 
		"${PROJECT_SOURCE_DIR}/src"
)
target_sources(
	glpp_imgui
	PUBLIC
		imgui.hpp
	PRIVATE
		imgui.cpp
)

target_link_libraries(
	glpp_imgui
	PUBLIC
		glpp::glpp
)

find_package(imgui REQUIRED)
target_link_libraries(
	glpp_imgui
	PUBLIC
		imgui::imgui
)

if (USING_CONAN)
	add_library(imgui_bindings OBJECT)
	add_library(imgui::imgui_bindings ALIAS imgui_bindings)
	target_include_directories(
		imgui_bindings
		PUBLIC
			"${PROJECT_BINARY_DIR}/bindings"
	)
	set_target_properties(
			imgui_bindings

			PROPERTIES
			VERSION "${BUTTONS_VERSION}"
			SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}"
	)
	target_sources(
		imgui_bindings
		PUBLIC
			"${PROJECT_BINARY_DIR}/bindings/imgui_impl_glfw.h"
			"${PROJECT_BINARY_DIR}/bindings/imgui_impl_opengl3.h"
		PRIVATE
			"${PROJECT_BINARY_DIR}/bindings/imgui_impl_glfw.cpp"
			"${PROJECT_BINARY_DIR}/bindings/imgui_impl_opengl3.cpp"
	)

	find_package(glfw REQUIRED)
	target_link_libraries(
		imgui_bindings
		PRIVATE
			glad::glad
			glfw::glfw
			imgui::imgui
	)

	target_link_libraries(
		glpp_imgui
		PRIVATE
			imgui::imgui_bindings
	)
endif()
