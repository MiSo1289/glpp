#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>

#include <boost/signals2.hpp>
#include "glpp/error.hpp"
#include "glpp/glfw/glfw.hpp"
#include "glpp/glfw/keys.hpp"
#include "glpp/glfw/event_handler_context.hpp"

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

    class Window
    {
      public:
        // Throws glpp::glfw::GlfwError
        Window(
            Glfw& glfw,
            WindowMode window_mode,
            std::string const& title);

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

        [[nodiscard]] auto on_key(std::function<void(KeyEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_button(std::function<void(MouseButtonEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_move(std::function<void(MouseMoveEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_scroll(std::function<void(MouseScrollEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto api_ptr() noexcept -> GLFWwindow*;

      private:
        struct Deleter
        {
            void operator()(GLFWwindow* glfw_window) const noexcept;
        };

		friend class EventHandlerContext;

        std::unique_ptr<GLFWwindow, Deleter> glfw_window_;
        boost::signals2::signal<void(KeyEvent)> key_signal_;
        boost::signals2::signal<void(MouseButtonEvent)> mouse_button_signal_;
        boost::signals2::signal<void(MouseMoveEvent)> mouse_move_signal_;
        boost::signals2::signal<void(MouseScrollEvent)> mouse_scroll_signal_;
        EventHandlerContext event_handler_context_;
    };
}  // namespace glpp::glfw
