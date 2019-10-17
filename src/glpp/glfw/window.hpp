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

        void trigger_key_event(KeyEvent event) const;

        [[nodiscard]] auto api_ptr() -> GLFWwindow*
        {
            return glfw_window_.get();
		}

      private:
        struct Deleter
        {
            void operator()(GLFWwindow* glfw_window) const noexcept;
        };

        std::unique_ptr<GLFWwindow, Deleter> glfw_window_;
        boost::signals2::signal<void(KeyEvent)> key_signal_;
    };
}  // namespace glpp::glfw
