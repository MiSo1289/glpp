add_library(glpp_glfw)
add_library(glpp::glpp_glfw ALIAS glpp_glfw)

target_compile_features(
	glpp_glfw
	PUBLIC 
		cxx_std_17
)
target_include_directories(
	glpp_glfw 
	PUBLIC 
		"${PROJECT_SOURCE_DIR}/src"
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
		glpp::glpp
)

if (USING_CONAN)
	find_package(glfw REQUIRED)
	find_package(Boost REQUIRED)
	target_link_libraries(
		glpp_glfw
		PUBLIC
			Boost::Boost
			glfw::glfw
	)
else()
	find_package(glfw3 REQUIRED)
	target_link_libraries(
		glpp_glfw
		PUBLIC
			glfw3
	)
endif()
