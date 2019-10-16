#include "glpp/gl.hpp"

#include "glpp/error.hpp"

namespace glpp
{
    void init()
    {
        if (!gladLoadGL())
        {
            throw InitError{"Failed to load OpenGL"};
        }
    }
}