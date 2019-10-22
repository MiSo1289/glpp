#pragma once

#include "glpp/glfw/keys.hpp"

namespace glpp::glfw
{
    struct KeyEvent
    {
        KeyCode key;
        int scan_code;
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

    struct MouseScrollEvent
    {
        double xoff;
        double yoff;
    };
}
