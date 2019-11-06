#pragma once

#include "glpp/primitive_types.hpp"

namespace glpp
{
    // Has to be called after an OpenGL context has been initialized
    // If using GLFW, see glpp::glfw::Glfw::load_gl()
    //
    // Throws glpp::InitError
    void load_gl();

	void set_viewport_size(Int32 x, Int32 y, Size width, Size height) noexcept;
}  // namespace glpp