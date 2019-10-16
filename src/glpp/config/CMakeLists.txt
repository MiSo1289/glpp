find_package(nlohmann_json CONFIG REQUIRED)

add_library(glpp_config)
add_library(glpp::glpp_config ALIAS glpp_config)

target_compile_features(
	glpp_config
	PUBLIC 
		cxx_std_17
)
target_include_directories(
	glpp_config 
	PUBLIC 
		"${PROJECT_SOURCE_DIR}/src"
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
		nlohmann_json::nlohmann_json
		glpp::glpp
)