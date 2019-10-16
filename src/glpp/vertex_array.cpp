#include "glpp/vertex_array.hpp"

namespace glpp
{
    void VertexArray::unbind_attribute_buffer(AttributeLocation attribute_loc) noexcept
    {
        auto vao_bind = ScopedBind{*this};
        glDisableVertexAttribArray(attribute_loc.value);
    }
}