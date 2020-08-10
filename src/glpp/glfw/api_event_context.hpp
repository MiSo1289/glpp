#pragma once

#include <exception>

#include <boost/signals2.hpp>
#include "glpp/glfw/events.hpp"

namespace glpp::glfw
{
    class ApiEventContext
    {
      public:
        [[nodiscard]] auto on_api_cursor_pos(std::function<void(double, double)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_mouse_button(std::function<void(int, int, int)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_key(std::function<void(int, int, int, int)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_char(std::function<void(unsigned)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_drop(std::function<void(int, char const**)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_scroll(std::function<void(double, double)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_framebuffer_size(std::function<void(int, int)> cb)
            -> boost::signals2::connection;

        [[nodiscard]] auto on_api_content_scale(std::function<void(float, float)> cb)
        -> boost::signals2::connection;

        void rethrow_any_error();

        void trigger_cursor_pos_event(double x, double y) noexcept;

        void trigger_mouse_button_event(int button, int action, int modifiers) noexcept;

        void trigger_key_event(int key, int scancode, int action, int modifiers) noexcept;

        void trigger_char_event(unsigned codepoint) noexcept;

        void trigger_drop_event(int count, char const** filenames) noexcept;

        void trigger_scroll_event(double x, double y) noexcept;

        void trigger_framebuffer_size_event(int width, int height) noexcept;

        void trigger_content_scale_event(float hor_scale, float vert_scale) noexcept;

      private:
        std::exception_ptr error_ = nullptr;
        boost::signals2::signal<void(double, double)> api_cursor_pos_signal_;
        boost::signals2::signal<void(int, int, int)> api_mouse_button_signal_;
        boost::signals2::signal<void(int, int, int, int)> api_key_signal_;
        boost::signals2::signal<void(unsigned)> api_char_signal_;
        boost::signals2::signal<void(int, char const**)> api_drop_signal_;
        boost::signals2::signal<void(double, double)> api_scroll_signal_;
        boost::signals2::signal<void(int, int)> api_framebuffer_size_signal_;
        boost::signals2::signal<void(float, float)> api_content_scale_signal_;

        template <typename Signal, typename... Args>
        void try_trigger_signal(Signal& signal, Args... args) noexcept;
    };
}  // namespace glpp::glfw