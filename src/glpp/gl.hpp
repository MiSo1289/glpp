#pragma once

#include "glpp/primitive_types.hpp"

namespace glpp
{
    // Has to be called after an OpenGL context has been initialized
    // If using GLFW, see glpp::glfw::Glfw::load_gl()
    //
    // Throws glpp::InitError
    void load_gl();

    struct ViewportSize
    {
        Int32 x;
        Int32 y;
        Size width;
        Size height;
    };

    void set_viewport_size(ViewportSize size) noexcept;

    void set_viewport_size(
        Int32 x,
        Int32 y,
        Size width,
        Size height) noexcept;
        
    [[nodiscard]] auto get_viewport_size() noexcept -> ViewportSize;
}  // namespace glpp