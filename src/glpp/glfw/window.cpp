#include "glpp/glfw/window.hpp"

#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <gsl/gsl_util>
#include "glpp/error.hpp"
#include "glpp/gl.hpp"
#include "glpp/glfw/error.hpp"

namespace
{
    extern "C" void glpp_glfw_cursor_pos_callback(
        GLFWwindow* const window,
        double const x,
        double const y) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_cursor_pos_event(x, y);
    }

    extern "C" void glpp_glfw_mouse_button_callback(
        GLFWwindow* const window,
        int const button,
        int const action,
        int const modifiers) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_mouse_button_event(button, action, modifiers);
    }

    extern "C" void glpp_glfw_key_callback(
        GLFWwindow* const window,
        int const key,
        int const scancode,
        int const action,
        int const modifiers) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_key_event(key, scancode, action, modifiers);
    }

    extern "C" void glpp_glfw_char_callback(
        GLFWwindow* const window,
        unsigned const codepoint) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_char_event(codepoint);
    }

    extern "C" void glpp_glfw_drop_callback(
        GLFWwindow* const window,
        int const count,
        char const** const filenames) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_drop_event(count, filenames);
    }

    extern "C" void glpp_glfw_scroll_callback(
        GLFWwindow* const window,
        double const x,
        double const y) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_scroll_event(x, y);
    }

    extern "C" void glpp_glfw_framebuffer_size_callback(
        GLFWwindow* const window,
        int const width,
        int const height) noexcept
    {
        auto* const user_ptr = glpp::glfw::unchecked_api_invoke(
            &glfwGetWindowUserPointer,
            window);

        assert(user_ptr != nullptr);

        static_cast<glpp::glfw::ApiEventContext*>(user_ptr)
            ->trigger_framebuffer_size_event(width, height);
    }
}  // namespace

namespace glpp::glfw
{
    Window::Window(
        Glfw& glfw,
        WindowMode const window_mode,
        std::string const& title,
        FramebufferAutomaticResize const framebuffer_resize)
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
      , api_cursor_pos_cb_conn_{
            api_event_context_.on_api_cursor_pos([this](double x, double y) {
                cursor_pos_signal_({x, y});
            }),
        }
      , api_mouse_button_cb_conn_{
            api_event_context_.on_api_mouse_button([this](int button, int action, int modifiers) {
                mouse_button_signal_({
                    static_cast<glpp::glfw::MouseButton>(button),
                    static_cast<glpp::glfw::KeyAction>(action),
                    static_cast<glpp::glfw::KeyMod>(modifiers),
                });
            }),
        }
      , api_key_cb_conn_{
            api_event_context_.on_api_key([this](int key, int scancode, int action, int modifiers) {
                key_signal_({
                    static_cast<glpp::glfw::KeyCode>(key),
                    scancode,
                    static_cast<glpp::glfw::KeyAction>(action),
                    static_cast<glpp::glfw::KeyMod>(modifiers),
                });
            }),
        }
      , api_char_cb_conn_{
            api_event_context_.on_api_char([this](unsigned codepoint) {
                char_signal_({codepoint});
            }),
        }
      , api_drop_cb_conn_{
            api_event_context_.on_api_drop([this](int count, char const** filenames) {
                auto files = std::vector<std::filesystem::path>();
                std::copy(filenames, filenames + count, std::back_inserter(files));
                drop_signal_({std::move(files)});
            }),
        }
      , api_scroll_cb_conn_{
            api_event_context_.on_api_scroll([this](double x, double y) {
                scroll_signal_({x, y});
            }),
        }
      , api_framebuffer_size_cb_conn_{
            api_event_context_.on_api_framebuffer_size([this](int width, int height) {
                framebuffer_size_signal_({width, height});
            }),
        }
    {
        if (!glfw_window_)
        {
            throw InitError{"Creating window failed"};
        }

        if (framebuffer_resize == FramebufferAutomaticResize::enabled)
        {
            framebuffer_size_gl_viewport_conn_
                = on_framebuffer_size([](auto const event) {
                      set_viewport_size(0, 0, event.width, event.height);
                  });
        }

        make_context_current();
        glfw.load_gl();

        checked_api_invoke(
            &glfwSetWindowUserPointer,
            glfw_window_.get(),
            &api_event_context_);

        checked_api_invoke(
            &glfwSetCursorPosCallback,
            glfw_window_.get(),
            &glpp_glfw_cursor_pos_callback);
        checked_api_invoke(
            &glfwSetMouseButtonCallback,
            glfw_window_.get(),
            &glpp_glfw_mouse_button_callback);
        checked_api_invoke(
            &glfwSetKeyCallback,
            glfw_window_.get(),
            &glpp_glfw_key_callback);
        checked_api_invoke(
            &glfwSetCharCallback,
            glfw_window_.get(),
            &glpp_glfw_char_callback);
        checked_api_invoke(
            &glfwSetDropCallback,
            glfw_window_.get(),
            &glpp_glfw_drop_callback);
        checked_api_invoke(
            &glfwSetScrollCallback,
            glfw_window_.get(),
            &glpp_glfw_scroll_callback);
        checked_api_invoke(
            &glfwSetFramebufferSizeCallback,
            glfw_window_.get(),
            &glpp_glfw_framebuffer_size_callback);
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
        api_event_context_.rethrow_any_error();
    }

    void Window::swap_buffers()
    {
        checked_api_invoke(&glfwSwapBuffers, glfw_window_.get());
    }

    void Window::make_context_current()
    {
        checked_api_invoke(
            &glfwMakeContextCurrent,
            glfw_window_.get());
    }

    auto Window::framebuffer_size() const -> std::pair<int, int>
    {
        auto width = int{};
        auto height = int{};
        checked_api_invoke(
            &glfwGetFramebufferSize,
            glfw_window_.get(),
            &width,
            &height);

        return {width, height};
    }

    auto Window::on_cursor_pos(std::function<void(CursorPosEvent const&)> cb)
        -> boost::signals2::connection
    {
        return cursor_pos_signal_.connect(std::move(cb));
    }

    auto Window::on_mouse_button(std::function<void(MouseButtonEvent const&)> cb)
        -> boost::signals2::connection
    {
        return mouse_button_signal_.connect(std::move(cb));
    }

    auto Window::on_key(std::function<void(KeyEvent const&)> cb)
        -> boost::signals2::connection
    {
        return key_signal_.connect(std::move(cb));
    }

    auto Window::on_char(std::function<void(CharEvent const&)> cb)
        -> boost::signals2::connection
    {
        return char_signal_.connect(std::move(cb));
    }

    auto Window::on_drop(std::function<void(DropEvent const&)> cb)
        -> boost::signals2::connection
    {
        return drop_signal_.connect(std::move(cb));
    }

    auto Window::on_scroll(std::function<void(ScrollEvent const&)> cb)
        -> boost::signals2::connection
    {
        return scroll_signal_.connect(std::move(cb));
    }

    auto Window::on_framebuffer_size(std::function<void(FrameBufferSizeEvent const&)> cb)
        -> boost::signals2::connection
    {
        return framebuffer_size_signal_.connect(std::move(cb));
    }

    auto Window::api_ptr() noexcept -> GLFWwindow*
    {
        return glfw_window_.get();
    }

    auto Window::api_event_context() noexcept -> ApiEventContext&
    {
        return api_event_context_;
    }

    void Window::Deleter::operator()(GLFWwindow* glfw_window) const noexcept
    {
        unchecked_api_invoke(&glfwDestroyWindow, glfw_window);
    }
}  // namespace glpp::glfw
