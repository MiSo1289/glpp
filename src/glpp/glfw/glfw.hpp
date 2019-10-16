#pragma once

#include <atomic>
#include <functional>
#include <string>

#include <boost/signals2.hpp>
// The include order is important here.
// It is therefore recommended to always include this header
// instead of including glfw directly.
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace glpp::glfw
{
    struct ErrorEvent
    {
        int err_number;
        std::string message;
    };

    class Glfw
    {
      public:
        [[nodiscard]] static auto get_instance() -> Glfw*
        {
            return instance;
        }

        // Only one instance can exist simultaneously.
        // If an instance does not exist, most glfw calls will fail.
        //
        // Throws glpp::InitError.
        Glfw();

        Glfw(Glfw const&) = delete;
        Glfw(Glfw&&) = delete;

        ~Glfw() noexcept;

        Glfw& operator=(Glfw const&) = delete;
        Glfw& operator=(Glfw&&) = delete;

        // Has to be called after a GLFW window has been created
        // and is the current context.
        // Called automatically when creating a glpp::glfw::Window.
        //
        // Throws glpp::InitError
        void load_gl();

        [[nodiscard]] auto on_error(std::function<void(ErrorEvent const&)> cb)
            -> boost::signals2::connection
        {
            return error_signal_.connect(std::move(cb));
        }

        void trigger_error_event(ErrorEvent const& event)
        {
            error_signal_(event);
        }

      private:
        inline static auto instance = std::atomic<Glfw*>{nullptr};

        boost::signals2::signal<void(ErrorEvent const&)> error_signal_;
    };

    // Has to be before called any GLFW functions.
    // Called automatically when creating a glpp::glfw::Window.
    //
    // Throws glpp::InitError
    void init_glfw();

    // Has to be called after a GLFW window has been created
    // and is the current context.
    // Called automatically when creating a glpp::glfw::Window.
    //
    // Throws glpp::InitError
    void load_gl_with_glfw();
}  // namespace glpp::glfw