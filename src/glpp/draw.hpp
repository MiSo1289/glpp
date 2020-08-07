#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glpp/buffer.hpp"
#include "glpp/primitive_types.hpp"
#include "glpp/scoped_bind.hpp"

namespace glpp
{
    void clear_color(glm::vec4 color) noexcept;

    void clear_stencil(Int32 stencil) noexcept;

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

    void draw(
        DrawPrimitive primitive,
        Size num_vertices,
        Int32 first = 0) noexcept;

    template <typename IndexType>
    void draw_indexed(
        DrawPrimitive const primitive,
        IndexBufferView<IndexType> const indices) noexcept
    {
        auto indices_binding = ScopedBind{indices};
        glDrawElements(
            static_cast<Enum>(primitive),
            static_cast<Size>(indices.size()),
            primitive_type_enumerator_v<IndexType>,
            reinterpret_cast<void*>(indices.offset() * sizeof(IndexType)));
    }

    void draw_points(
        Size num_points,
        Int32 first = 0,
        Float32 point_size = 1.0f) noexcept;

}  // namespace glpp
