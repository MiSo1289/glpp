#include "glpp/glfw/window.hpp"

#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <gsl/gsl_util>
#include <magic_enum.hpp>
#include "glpp/error.hpp"
#include "glpp/glfw/error.hpp"

namespace
{
    extern "C" void glpp_glfw_key_callback(
        GLFWwindow* const window,
        int const key,
        int const scancode,
        int const action,
        int const mods) noexcept
    {
        try
        {
            auto* const user_ptr = glpp::glfw::checked_api_invoke(
                &glfwGetWindowUserPointer,
                window);

            assert(user_ptr != nullptr);
            static_cast<glpp::glfw::Window*>(user_ptr)
                ->trigger_key_event({
                    magic_enum::enum_cast<glpp::glfw::KeyCode>(key)
                        .value_or(glpp::glfw::KeyCode::unknown),
                    magic_enum::enum_cast<glpp::glfw::KeyAction>(action)
                        .value(),
                    glpp::glfw::KeyMod{gsl::narrow<unsigned>(mods)},
                });
        }
        catch (std::exception const& error)
        {
            std::fprintf(
                stderr,
                "Uncaught exception when processing GLFW key event: %s\n",
                error.what());
        }
        catch (...)
        {
            std::fprintf(
                stderr,
                "Unknown uncaught exception when processing GLFW key event\n");
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
          checked_api_invoke(
              &glfwCreateWindow,
              window_mode.width,
              window_mode.height,
              title.c_str(),
              window_mode.window_type == WindowType::fullscreen
                  ? glfwGetPrimaryMonitor()
                  : nullptr,
              nullptr),
      }
    {
        if (!glfw_window_)
        {
            throw InitError{"Creating window failed"};
        }

        checked_api_invoke(
            &glfwMakeContextCurrent,
            glfw_window_.get());
        glfw.load_gl();
        checked_api_invoke(
            &glfwSetWindowUserPointer,
            glfw_window_.get(),
            this);
        checked_api_invoke(
            &glfwSetKeyCallback,
            glfw_window_.get(),
            &glpp_glfw_key_callback);
    }

    auto Window::is_open() -> bool
    {
        return !checked_api_invoke(
            &glfwWindowShouldClose,
            glfw_window_.get());
    }

    void Window::update()
    {
        checked_api_invoke(&glfwSwapBuffers, glfw_window_.get());
        checked_api_invoke(&glfwPollEvents);
    }

    auto Window::on_key(std::function<void(KeyEvent)> cb)
        -> boost::signals2::connection
    {
        return key_signal_.connect(std::move(cb));
    }

    void Window::trigger_key_event(KeyEvent event)
    {
        key_signal_(event);
    }

    void Window::Deleter::operator()(GLFWwindow* glfw_window) const noexcept
    {
        checked_api_invoke(&glfwDestroyWindow, glfw_window);
    }
}  // namespace glpp::glfw