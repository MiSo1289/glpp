# glpp

C++ wrapper library for OpenGL and GLFW, with ImGui integration support. 

## Dependencies

The Conan and Vcpkg package managers are supported for installing dependencies.

### Using Conan

Simply create the project in your cache, conan will take care of the rest.

### Using Vcpkg

The following packages must be installed:

* fmt
* glad
* glm
* magic-enum
* ms-gsl
* With BUILD_CONFIG:
	* nlohmann-json
* With BUILD_GLFW:
	* boost-signals2
	* glfw3
* With BUILD_IMGUI:
	* imgui\[core,glfw-binding,opengl3-glad-binding\]
* With BUILD_EXAMPLES:
	* clara