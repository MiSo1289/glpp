#include "glpp/draw.hpp"

namespace glpp
{
    void clear_color(glm::vec4 const color) noexcept
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void clear_depth(Float32 const depth) noexcept
    {
        glClearDepthf(depth);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void clear_stencil(Int32 const stencil) noexcept
    {
        glClearStencil(stencil);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void draw(
        DrawPrimitive const primitive,
        Size const num_vertices,
        Int32 const first) noexcept
    {
        glDrawArrays(
            static_cast<Enum>(primitive),
            first,
            num_vertices);
    }

    void draw_points(
        Size const num_points,
        Int32 const first,
        Float32 const point_size) noexcept
    {
        glPointSize(point_size);
        draw(DrawPrimitive::points, num_points, first);
        glPointSize(1.0f);
    }
}  // namespace glpp
