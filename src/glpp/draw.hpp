#pragma once

#include "glpp/buffer.hpp"
#include "glpp/primitive_types.hpp"

namespace glpp
{
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
        int num_vertices,
        int first = 0) noexcept;

    template <typename IndexType>
    void draw_indexed(
        DrawPrimitive const primitive,
        IndexBufferView<IndexType> const indices) noexcept
    {
        auto indices_binding = ScopedBind{indices};
        glDrawElements(
            static_cast<Enum>(primitive),
            static_cast<Size>(indices.size()),
            primitive_type_enumerator_v<IndexT>,
            reinterpret_cast<void*>(indices.offset() * sizeof(IndexType)));
    }
}  // namespace glpp
