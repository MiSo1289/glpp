#pragma once

#include <exception>

#include "glpp/glfw/events.hpp"

namespace glpp::glfw
{
    class Window;

    class EventHandlerContext
    {
      public:
        explicit EventHandlerContext(Window& window) noexcept
          : window_{window} {}

        void rethrow_any_error();

        void trigger_key_event(KeyEvent event) noexcept;

        void trigger_mouse_button_event(MouseButtonEvent event) noexcept;

        void trigger_mouse_move_event(MouseMoveEvent event) noexcept;

        void trigger_mouse_scroll_event(MouseScrollEvent event) noexcept;

      private:
        Window& window_;
        std::exception_ptr error_ = nullptr;

        template <typename Signal, typename Event>
        void try_trigger_event(Signal& signal, Event event) noexcept;
    };
}  // namespace glpp::glfw