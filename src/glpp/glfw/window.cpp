#include "glpp/glfw/window.hpp"

#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <gsl/gsl_util>
#include "glpp/error.hpp"
#include "glpp/glfw/error.hpp"

namespace
{
    extern "C" void glpp_glfw_key_callback(
        GLFWwindow* const window,
        int const key,
        int const scan_code,
        int const action,
        int const mods) noexcept
    {
        auto* const user_ptr = glpp::glfw::checked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::EventHandlerContext*>(user_ptr)
            ->trigger_key_event({
                static_cast<glpp::glfw::KeyCode>(key),
                scan_code,
                static_cast<glpp::glfw::KeyAction>(action),
                static_cast<glpp::glfw::KeyMod>(mods),
            });
    }

    extern "C" void glpp_glfw_mouse_button_callback(
        GLFWwindow* const window,
        int const button,
        int const action,
        int const mods) noexcept
    {
        auto* const user_ptr = glpp::glfw::checked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);
        static_cast<glpp::glfw::EventHandlerContext*>(user_ptr)
            ->trigger_mouse_button_event({
                static_cast<glpp::glfw::MouseButton>(button),
                static_cast<glpp::glfw::KeyAction>(action),
                static_cast<glpp::glfw::KeyMod>(mods),
            });
    }

    extern "C" void glpp_glfw_mouse_move_callback(
        GLFWwindow* const window,
        double const xpos,
        double const ypos) noexcept
    {
        auto* const user_ptr = glpp::glfw::checked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);
        static_cast<glpp::glfw::EventHandlerContext*>(user_ptr)
            ->trigger_mouse_move_event({
                xpos,
                ypos,
            });
    }

    extern "C" void glpp_glfw_mouse_scroll_callback(
        GLFWwindow* const window,
        double const xoff,
        double const yoff) noexcept
    {
        auto* const user_ptr = glpp::glfw::checked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);
        static_cast<glpp::glfw::EventHandlerContext*>(user_ptr)
            ->trigger_mouse_scroll_event({
                xoff,
                yoff,
            });
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
      , event_handler_context_{*this}
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
            &event_handler_context_);

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
        checked_api_invoke(
            &glfwSetScrollCallback,
            glfw_window_.get(),
            &glpp_glfw_mouse_scroll_callback);
    }

    auto Window::is_open() const -> bool
    {
        return !checked_api_invoke(
            &glfwWindowShouldClose,
            glfw_window_.get());
    }

    void Window::poll_events()
    {
        checked_api_invoke(&glfwPollEvents);
        event_handler_context_.rethrow_any_error();
    }

    void Window::swap_buffers()
    {
        checked_api_invoke(&glfwSwapBuffers, glfw_window_.get());
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

    auto Window::on_mouse_scroll(std::function<void(MouseScrollEvent)> cb)
        -> boost::signals2::connection
    {
        return mouse_scroll_signal_.connect(std::move(cb));
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