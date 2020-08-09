add_library(glpp)
add_library(glpp::glpp ALIAS glpp)

target_compile_features(
	glpp 
	PUBLIC 
		cxx_std_17
)
target_compile_definitions(
	glpp
	PUBLIC
		NOMINMAX
)
target_include_directories(
	glpp 
	PUBLIC 
		"${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
	glpp
	PROPERTIES
		VERSION "${CMAKE_PROJECT_VERSION}"
		SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}"
)
target_sources(
	glpp
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

find_package(fmt REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(
	glpp
	PUBLIC
		OpenGL::GL
		fmt::fmt
		glad::glad
)

if (USING_CONAN)
	find_package(magic_enum REQUIRED)
	find_package(Microsoft.GSL REQUIRED)
	target_link_libraries(
		glpp
		PUBLIC
			glm::glm
			magic_enum::magic_enum
			Microsoft.GSL::Microsoft.GSL
	)
else()
	target_link_libraries(
		glpp
			PUBLIC
			glm
	)
endif()

option(BUILD_CONFIG "Build the glpp_config library" ON)
if (BUILD_CONFIG)
	add_subdirectory(config)
endif()

option(BUILD_GLFW "Build the glpp_glfw library" ON)
if (BUILD_GLFW)
	add_subdirectory(glfw)

	option(BUILD_IMGUI "Build the glpp_imgui library" ON)
	if (BUILD_IMGUI)
		add_subdirectory(imgui)
	endif()
endif()