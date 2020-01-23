#include "glpp/gl.hpp"

#include "glpp/error.hpp"

namespace
{
    thread_local glpp::ViewportSize viewport_size = {};
}

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
        viewport_size = size;
        glViewport(size.x, size.y, size.width, size.height);
    }

    auto get_viewport_size() noexcept -> ViewportSize
    {
        return viewport_size;
    }
}  // namespace glpp
