#include "glpp/glfw/api_event_context.hpp"

#include <utility>

#include "glpp/glfw/window.hpp"

namespace glpp::glfw
{
    template <typename Signal, typename... Args>
    void ApiEventContext::try_trigger_signal(Signal& signal, Args... args) noexcept
    {
        if (!error_)
        {
            try
            {
                signal(args...);
            }
            catch (...)
            {
                error_ = std::current_exception();
            }
        }
    }

    auto ApiEventContext::on_api_cursor_pos(std::function<void(double, double)> cb)
        -> boost::signals2::connection
    {
        return api_cursor_pos_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_mouse_button(std::function<void(int, int, int)> cb)
        -> boost::signals2::connection
    {
        return api_mouse_button_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_key(std::function<void(int, int, int, int)> cb)
        -> boost::signals2::connection
    {
        return api_key_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_char(std::function<void(unsigned)> cb)
        -> boost::signals2::connection
    {
        return api_char_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_drop(std::function<void(int, char const**)> cb)
        -> boost::signals2::connection
    {
        return api_drop_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_scroll(std::function<void(double, double)> cb)
        -> boost::signals2::connection
    {
        return api_scroll_signal_.connect(std::move(cb));
    }

    auto ApiEventContext::on_api_framebuffer_size(std::function<void(int, int)> cb)
        -> boost::signals2::connection
    {
        return api_framebuffer_size_signal_.connect(std::move(cb));
    }

    void ApiEventContext::rethrow_any_error()
    {
        if (error_)
        {
            std::rethrow_exception(std::exchange(error_, nullptr));
        }
    }

    void ApiEventContext::trigger_cursor_pos_event(double x, double y) noexcept
    {
        try_trigger_signal(api_cursor_pos_signal_, x, y);
    }

    void ApiEventContext::trigger_mouse_button_event(int button, int action, int modifiers) noexcept
    {
        try_trigger_signal(api_mouse_button_signal_, button, action, modifiers);
    }

    void ApiEventContext::trigger_key_event(int key, int scancode, int action, int modifiers) noexcept
    {
        try_trigger_signal(api_key_signal_, key, scancode, action, modifiers);
    }

    void ApiEventContext::trigger_char_event(unsigned codepoint) noexcept
    {
        try_trigger_signal(api_char_signal_, codepoint);
    }

    void ApiEventContext::trigger_drop_event(int count, char const** filenames) noexcept
    {
        try_trigger_signal(api_drop_signal_, count, filenames);
    }

    void ApiEventContext::trigger_scroll_event(double x, double y) noexcept
    {
        try_trigger_signal(api_scroll_signal_, x, y);
    }

    void ApiEventContext::trigger_framebuffer_size_event(int width, int height) noexcept
    {
        try_trigger_signal(api_framebuffer_size_signal_, width, height);
    }
}  // namespace glpp::glfw