#pragma once

#include <stdexcept>

#include <glad/glad.h>
#include "glpp/buffer.hpp"
#include "glpp/primitive_types.hpp"
#include "glpp/scoped_bind.hpp"
#include "glpp/traits.hpp"

namespace glpp
{
    // Has to be called after an OpenGL context has been initialized
    // If using GLFW, see glpp::glfw::Glfw::load_gl()
    //
    // Throws glpp::InitError
    void load_gl();
}  // namespace glpp