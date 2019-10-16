#include "glpp/glfw/glfw.hpp"

#include <stdexcept>

#include <spdlog/spdlog.h>
#include "glpp/error.hpp"

namespace
{
    extern "C" void glpp_glfw_error_callback(
        int const error,
        char const* const description)
    {
        try
        {
            if (auto* glfw = glpp::glfw::Glfw::get_instance())
            {
                glfw->trigger_error_event({
                    error,
                    description,
                });
            }
            else
            {
                spdlog::error(
                    "GLFW error was raised, but GLFW is not initialized\n"
                    "Error: {0}; Message: {1}",
                    error,
                    description);
            }
        }
        catch (std::exception const& error)
        {
            spdlog::error(
                "Uncaught exception when processing GLFW error event: {0}",
                error.what());
        }
        catch (...)
        {
            spdlog::error(
                "Unknown uncaught exception when processing GLFW error event");
        }
    }
}  // namespace

namespace glpp::glfw
{
    Glfw::Glfw()
    {
        if (instance)
        {
            throw InitError{"GLFW already initialized"};
        }
        instance = this;

        glfwSetErrorCallback(&glpp_glfw_error_callback);

        if (!glfwInit())
        {
            throw InitError{"Failed to init GLFW"};
        }
    }

    Glfw::~Glfw() noexcept
    {
        glfwTerminate();
        glfwSetErrorCallback(nullptr);

		instance = nullptr;
	}

    void Glfw::load_gl()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw InitError{"Failed to load OpenGL"};
        }
    }
}  // namespace glpp::glfw