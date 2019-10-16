find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

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
target_sources(
	glpp
	PUBLIC
		buffer.hpp
		framebuffer.hpp
		gl.hpp
		id.hpp
		primitive_types.hpp
		scoped_bind.hpp
		shader.hpp
		shader_program.hpp
		traits.hpp
		uniform.hpp
		vertex_array.hpp
	PRIVATE
		buffer.cpp
		framebuffer.cpp
		gl.cpp
		id.cpp
		primitive_types.cpp
		scoped_bind.cpp
		shader.cpp
		shader_program.cpp
		traits.cpp
		uniform.cpp
		vertex_array.cpp
)
target_link_libraries(
	glpp 
	PUBLIC 
		fmt::fmt
		glad::glad
		glm
)

option(BUILD_CONFIG "Build the glpp_config library" ON)
if (BUILD_CONFIG)
	add_subdirectory(config)
endif()

option(BUILD_GLFW "Build the glpp_glfw library" ON)
if (BUILD_GLFW)
	add_subdirectory(glfw)
endif()