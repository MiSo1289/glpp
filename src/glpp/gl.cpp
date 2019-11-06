#include "glpp/gl.hpp"

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

    void set_viewport_size(Int32 x, Int32 y, Size width, Size height) noexcept
    {
        glViewport(x, y, width, height);
	}
}  // namespace glpp
