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

    enum class DrawPrimitive : Enum
    {
        points = GL_POINTS,
        lines = GL_LINES,
        line_strip = GL_LINE_STRIP,
        line_loop = GL_LINE_LOOP,
        triangles = GL_TRIANGLES,
        triangle_strip = GL_TRIANGLE_STRIP,
        triangle_fan = GL_TRIANGLE_FAN,
    };

    template <typename IndexType>
    void draw_indexed(IndexBufferView<IndexType> indices, DrawPrimitive primitive) noexcept
    {
        auto indices_binding = ScopedBind{indices};
        glDrawElements(
            static_cast<Enum>(primitive),
            static_cast<Size>(indices.size()),
            primitive_type_enumerator_v<IndexT>,
            reinterpret_cast<void*>(indices.offset() * sizeof(IndexType)));
    }
}  // namespace glpp