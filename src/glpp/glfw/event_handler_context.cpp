#include "glpp/glfw/event_handler_context.hpp"

#include <utility>

#include "glpp/glfw/window.hpp"

namespace glpp::glfw
{
    template <typename Signal, typename Event>
    void EventHandlerContext::try_trigger_event(Signal& signal, Event const event) noexcept
    {
        if (!error_)
        {
            try
            {
                signal(event);
            }
            catch (...)
            {
                error_ = std::current_exception();
            }
        }
    }

    void EventHandlerContext::rethrow_any_error()
    {
        if (error_)
        {
            std::rethrow_exception(std::exchange(error_, nullptr));
        }
    }

    void EventHandlerContext::trigger_key_event(KeyEvent const event) noexcept
    {
        try_trigger_event(window_.key_signal_, event);
    }

    void EventHandlerContext::trigger_mouse_button_event(MouseButtonEvent const event) noexcept
    {
        try_trigger_event(window_.mouse_button_signal_, event);
    }

    void EventHandlerContext::trigger_mouse_move_event(MouseMoveEvent const event) noexcept
    {
        try_trigger_event(window_.mouse_move_signal_, event);
    }

    void EventHandlerContext::trigger_mouse_scroll_event(MouseScrollEvent const event) noexcept
    {
        try_trigger_event(window_.mouse_scroll_signal_, event);
    }
}  // namespace glpp::glfw