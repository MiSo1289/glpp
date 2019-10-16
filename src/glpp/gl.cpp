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
}