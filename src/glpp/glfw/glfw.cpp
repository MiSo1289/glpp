#include "glpp/glfw/glfw.hpp"

#include <stdexcept>

#include "glpp/error.hpp"
#include "glpp/glfw/error.hpp"

namespace glpp::glfw
{
    Glfw::Glfw()
    {
        auto expected = static_cast<Glfw*>(nullptr);
        if (!instance.compare_exchange_strong(expected, this))
        {
            throw InitError{"GLFW already initialized"};
        }

        install_error_handler();

        if (!checked_api_invoke(&glfwInit))
        {
            // Undo actions before throw as the destructor will not be called.
            uninstall_error_handler();
            instance = nullptr;

            throw InitError{"Failed to init GLFW"};
        }
    }

    Glfw::~Glfw() noexcept
    {
        unchecked_api_invoke(&glfwTerminate);
        uninstall_error_handler();

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