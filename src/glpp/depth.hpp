#pragma once

#include "glpp/primitive_types.hpp"

#include <glad/glad.h>

namespace glpp
{
    void clear_depth(Float32 depth = 1.0f) noexcept;

    enum class DepthBufferAccess
    {
        read,
        read_write,
    };

    enum class DepthPredicate : Enum
    {
        always = GL_ALWAYS,
        never = GL_NEVER,
        less = GL_LESS,
        equal = GL_EQUAL,
        less_equal = GL_LEQUAL,
        greater = GL_GREATER,
        not_equal = GL_NOTEQUAL,
        greater_equal = GL_GEQUAL,
    };

    class DepthMode
    {
      public:
        DepthMode(
            DepthPredicate const predicate = DepthPredicate::less,
            DepthBufferAccess const access = DepthBufferAccess::read_write)
          : predicate_{predicate}
          , access_{access} {}

        void bind() const noexcept;

        void unbind() const noexcept;

      private:
        DepthPredicate predicate_;
        DepthBufferAccess access_;
    };
}  // namespace glpp
