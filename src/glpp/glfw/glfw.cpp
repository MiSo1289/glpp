#include "glpp/glfw/glfw.hpp"

#include <stdexcept>

#include "glpp/error.hpp"
#include "glpp/glfw/error.hpp"

namespace glpp::glfw
{
    Glfw::Glfw()
    {
        if (instance)
        {
            throw InitError{"GLFW already initialized"};
        }
        instance = this;

        if (!checked_api_invoke(&glfwInit))
        {
            throw InitError{"Failed to init GLFW"};
        }
    }

    Glfw::~Glfw() noexcept
    {
        unchecked_api_invoke(&glfwTerminate);
        unchecked_api_invoke(&glfwSetErrorCallback, nullptr);

		instance = nullptr;
	}

    void Glfw::load_gl()
    {
        if (!checked_api_invoke(
			&gladLoadGLLoader,
			reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw InitError{"Failed to load OpenGL"};
        }
    }
}  // namespace glpp::glfw