#pragma once

#include <filesystem>
#include <vector>

#include "glpp/glfw/keys.hpp"

namespace glpp::glfw
{
    struct CursorPosEvent
    {
        double xpos;
        double ypos;
    };

    struct MouseButtonEvent
    {
        MouseButton button;
        KeyAction action;
        KeyMod mods;
    };

    struct KeyEvent
    {
        KeyCode key;
        int scan_code;
        KeyAction action;
        KeyMod mods;
    };

    struct CharEvent
    {
        unsigned codepoint;
    };

    struct DropEvent
    {
        std::vector<std::filesystem::path> files;
    };

    struct ScrollEvent
    {
        double xoff;
        double yoff;
    };

    struct FrameBufferSizeEvent
    {
        int width;
        int height;
    };

    struct ContentScaleEvent
    {
        float hor_scale;
        float vert_scale;
    };
}  // namespace glpp::glfw
