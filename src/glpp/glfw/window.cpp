#include "glpp/glfw/window.hpp"

#include <stdexcept>

#include <spdlog/spdlog.h>
#include "glpp/error.hpp"

namespace
{
    extern "C" void glpp_glfw_key_callback(
        GLFWwindow* const window,
        int const key,
        int const scancode,
        int const action,
        int const mods)
    {
        try
        {
            static_cast<glpp::glfw::Window*>(glfwGetWindowUserPointer(window))
                ->trigger_key_event({
                    key,
                    scancode,
                    action,
                    mods,
                });
        }
        catch (std::exception const& error)
        {
            spdlog::error(
                "Uncaught exception when processing GLFW key event: {0}",
                error.what());
        }
        catch (...)
        {
            spdlog::error(
                "Unknown uncaught exception when processing GLFW key event");
        }
    }
}  // namespace

namespace glpp::glfw
{
    Window::Window(
		Glfw& glfw,
        WindowMode const window_mode,
        std::string const& title)
      : glfw_window_{
          glfwCreateWindow(
              window_mode.width,
              window_mode.height,
              title.c_str(),
              window_mode.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
              nullptr),
      }
    {
        if (!glfw_window_)
        {
            throw InitError{"Creating window failed"};
        }

        glfwMakeContextCurrent(glfw_window_.get());
        glfw.load_gl();
        glfwSetWindowUserPointer(glfw_window_.get(), this);
        glfwSetKeyCallback(glfw_window_.get(), &glpp_glfw_key_callback);
    }
}  // namespace glpp::glfw