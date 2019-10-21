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
        [[maybe_unused]] int const scancode,
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
                    magic_enum::enum_cast<glpp::glfw::KeyMod>(
                        gsl::narrow<std::uint8_t>(mods))
                        .value(),
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

    extern "C" void glpp_glfw_mouse_button_callback(
        GLFWwindow* const window,
        int const button,
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
                ->trigger_mouse_button_event({
                    magic_enum::enum_cast<glpp::glfw::MouseButton>(button)
                        .value_or(glpp::glfw::KeyCode::unknown),
                    magic_enum::enum_cast<glpp::glfw::KeyAction>(action)
                        .value(),
                    magic_enum::enum_cast<glpp::glfw::KeyMod>(
						gsl::narrow<std::uint8_t>(mods))
                        .value(),
                });
        }
        catch (std::exception const& error)
        {
            std::fprintf(
                stderr,
                "Uncaught exception when processing GLFW mouse button event: %s\n",
                error.what());
        }
        catch (...)
        {
            std::fprintf(
                stderr,
                "Unknown uncaught exception when processing GLFW mouse button event\n");
        }
    }

	extern "C" void glpp_glfw_mouse_move_callback(
        GLFWwindow* const window,
        double const xpos,
		double const ypos) noexcept
    {
        try
        {
            auto* const user_ptr = glpp::glfw::checked_api_invoke(
                &glfwGetWindowUserPointer,
                window);

            assert(user_ptr != nullptr);
            static_cast<glpp::glfw::Window*>(user_ptr)
                ->trigger_mouse_move_event({
                    xpos,
					ypos,
                });
        }
        catch (std::exception const& error)
        {
            std::fprintf(
                stderr,
                "Uncaught exception when processing GLFW mouse move event: %s\n",
                error.what());
        }
        catch (...)
        {
            std::fprintf(
                stderr,
                "Unknown uncaught exception when processing GLFW mouse move event\n");
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
        checked_api_invoke(
            &glfwSetMouseButtonCallback,
            glfw_window_.get(),
            &glpp_glfw_mouse_button_callback);
        checked_api_invoke(
            &glfwSetCursorPosCallback,
            glfw_window_.get(),
            &glpp_glfw_mouse_move_callback);
    }

    auto Window::is_open() const -> bool
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

    auto Window::on_mouse_button(std::function<void(MouseButtonEvent)> cb)
        -> boost::signals2::connection
    {
        return mouse_button_signal_.connect(std::move(cb));
    }

    auto Window::on_mouse_move(std::function<void(MouseMoveEvent)> cb)
        -> boost::signals2::connection
    {
        return mouse_move_signal_.connect(std::move(cb));
    }

    void Window::trigger_key_event(KeyEvent event) const
    {
        key_signal_(event);
    }

    void Window::trigger_mouse_button_event(MouseButtonEvent event) const
    {
        mouse_button_signal_(event);
    }

    void Window::trigger_mouse_move_event(MouseMoveEvent event) const
    {
        mouse_move_signal_(event);
    }

    auto Window::api_ptr() noexcept -> GLFWwindow*
    {
        return glfw_window_.get();
    }

    void Window::Deleter::operator()(GLFWwindow* glfw_window) const noexcept
    {
        unchecked_api_invoke(&glfwDestroyWindow, glfw_window);
    }
}  // namespace glpp::glfw