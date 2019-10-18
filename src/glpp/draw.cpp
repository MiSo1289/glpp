#include "glpp/draw.hpp"

namespace glpp
{
    void draw(
        DrawPrimitive const primitive,
        int const num_vertices,
        int const first) noexcept
    {
        glDrawArrays(
            static_cast<Enum>(primitive),
            first,
            num_vertices);
    }
}  // namespace glpp
