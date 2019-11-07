#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <boost/signals2.hpp>
#include "glpp/error.hpp"
#include "glpp/glfw/api_event_context.hpp"
#include "glpp/glfw/glfw.hpp"
#include "glpp/glfw/keys.hpp"

namespace glpp::glfw
{
    enum class WindowType
    {
        fullscreen,
        windowed,
    };

    struct WindowMode
    {
        WindowType window_type;
        int width;
        int height;
    };

    enum class FramebufferAutomaticResize
    {
        enabled,
        disabled,
    };

    class Window
    {
      public:
        // Throws glpp::glfw::GlfwError
        Window(
            Glfw& glfw,
            WindowMode window_mode,
            std::string const& title,
            FramebufferAutomaticResize framebuffer_resize
            = FramebufferAutomaticResize::enabled);

        Window(Window const&) = delete;
        Window(Window&&) = delete;

        auto operator=(Window const&) -> Window& = delete;
        auto operator=(Window &&) -> Window& = delete;

        // Throws glpp::glfw::GlfwError
        [[nodiscard]] auto is_open() const -> bool;

        // Poll events and pass them to user event handlers.
        // If a handler throws, no other handlers will get called
        // and all currently unprocessed events will be lost.
        // The exception will get propagated.
        //
        // Throws glpp::glfw::GlfwError
        void poll_events();

        // Display the back opengl buffer on screen.
        //
        // Throws glpp::glfw::GlfwError
        void swap_buffers();

        [[nodiscard]] auto framebuffer_size() const -> std::pair<int, int>;

        [[nodiscard]] auto on_cursor_pos(std::function<void(CursorPosEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_button(std::function<void(MouseButtonEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_key(std::function<void(KeyEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_char(std::function<void(CharEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_drop(std::function<void(DropEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_scroll(std::function<void(ScrollEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_framebuffer_size(std::function<void(FrameBufferSizeEvent const&)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto api_ptr() noexcept -> GLFWwindow*;

        [[nodiscard]] auto api_event_context() noexcept -> ApiEventContext&;

      private:
        struct Deleter
        {
            void operator()(GLFWwindow* glfw_window) const noexcept;
        };

        std::unique_ptr<GLFWwindow, Deleter> glfw_window_;
        ApiEventContext api_event_context_;
        boost::signals2::signal<void(CursorPosEvent const&)> cursor_pos_signal_;
        boost::signals2::signal<void(MouseButtonEvent const&)> mouse_button_signal_;
        boost::signals2::signal<void(KeyEvent const&)> key_signal_;
        boost::signals2::signal<void(CharEvent const&)> char_signal_;
        boost::signals2::signal<void(DropEvent const&)> drop_signal_;
        boost::signals2::signal<void(ScrollEvent const&)> scroll_signal_;
        boost::signals2::signal<void(FrameBufferSizeEvent const&)> framebuffer_size_signal_;
        boost::signals2::scoped_connection api_cursor_pos_cb_conn_;
        boost::signals2::scoped_connection api_mouse_button_cb_conn_;
        boost::signals2::scoped_connection api_key_cb_conn_;
        boost::signals2::scoped_connection api_char_cb_conn_;
        boost::signals2::scoped_connection api_drop_cb_conn_;
        boost::signals2::scoped_connection api_scroll_cb_conn_;
        boost::signals2::scoped_connection api_framebuffer_size_cb_conn_;
        boost::signals2::scoped_connection framebuffer_size_gl_viewport_conn_;
    };
}  // namespace glpp::glfw
