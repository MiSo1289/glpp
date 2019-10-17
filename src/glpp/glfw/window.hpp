#pragma once

#include <functional>
#include <memory>
#include <string>

#include <boost/signals2.hpp>
#include "glpp/error.hpp"
#include "glpp/glfw/glfw.hpp"

namespace glpp::glfw
{
    struct WindowMode
    {
        bool fullscreen;
        int width;
        int height;
    };

    struct KeyEvent
    {
        int key;
        int scancode;
        int action;
        int mods;
    };

    class Window
    {
      public:
        Window(
            Glfw& glfw,
            WindowMode window_mode,
            std::string const& title);

        [[nodiscard]] auto is_open() -> bool
        {
            return !glfwWindowShouldClose(glfw_window_.get());
        }

        void update()
        {
            glfwSwapBuffers(glfw_window_.get());
            glfwPollEvents();
        }

        [[nodiscard]] auto on_key(std::function<void(KeyEvent)> cb)
            -> boost::signals2::connection
        {
            return key_signal_.connect(std::move(cb));
        }

        void trigger_key_event(KeyEvent event)
        {
            key_signal_(event);
        }

      private:
        struct Deleter
        {
            void operator()(GLFWwindow* glfw_window) const noexcept
            {
                glfwDestroyWindow(glfw_window);
            }
        };

        std::unique_ptr<GLFWwindow, Deleter> glfw_window_;
        boost::signals2::signal<void(KeyEvent)> key_signal_;
    };
}  // namespace glpp::glfw
