#pragma once

#include <functional>
#include <memory>
#include <string>

#include <boost/signals2.hpp>
#include "glpp/error.hpp"
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

    struct KeyEvent
    {
        KeyCode key;
        KeyAction action;
        KeyMod mods;
    };

    struct MouseButtonEvent
    {
        MouseButton button;
        KeyAction action;
        KeyMod mods;
    };

    struct MouseMoveEvent
    {
        double xpos;
        double ypos;
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

        // Throws glpp::glfw::GlfwError
        void update();

        [[nodiscard]] auto on_key(std::function<void(KeyEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_button(std::function<void(MouseButtonEvent)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_mouse_move(std::function<void(MouseMoveEvent)> cb)
            -> boost::signals2::connection;

        void trigger_key_event(KeyEvent event) const;

        void trigger_mouse_button_event(MouseButtonEvent event) const;
        
		void trigger_mouse_move_event(MouseMoveEvent event) const;

        [[nodiscard]] auto api_ptr() noexcept -> GLFWwindow*;

      private:
        struct Deleter
        {
            void operator()(GLFWwindow* glfw_window) const noexcept;
        };

        std::unique_ptr<GLFWwindow, Deleter> glfw_window_;
        boost::signals2::signal<void(KeyEvent)> key_signal_;
        boost::signals2::signal<void(MouseButtonEvent)> mouse_button_signal_;
        boost::signals2::signal<void(MouseMoveEvent)> mouse_move_signal_;
    };
}  // namespace glpp::glfw
