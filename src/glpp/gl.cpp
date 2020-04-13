#include "glpp/gl.hpp"

#include <array>

#include "glpp/error.hpp"

namespace glpp
{
    void load_gl()
    {
        if (!gladLoadGL())
        {
            throw InitError{"Failed to load OpenGL"};
        }
    }

    void set_viewport_size(ViewportSize const size) noexcept
    {
        glViewport(size.x, size.y, size.width, size.height);
    }

    void set_viewport_size(
        Int32 const x,
        Int32 const y,
        Size const width,
        Size const height) noexcept
    {
        set_viewport_size(ViewportSize{x, y, width, height});
    }

    auto get_viewport_size() noexcept -> ViewportSize
    {
        auto values = std::array<Int32, 4>{};
        glGetIntegerv(GL_VIEWPORT, values.data());
        
        return {
            values[0],
            values[1],
            values[2],
            values[3],
        };
    }
}  // namespace glpp
