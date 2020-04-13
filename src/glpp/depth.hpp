#pragma once

#include <glad/glad.h>
#include "glpp/primitive_types.hpp"

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
        explicit DepthMode(
            DepthPredicate const predicate = DepthPredicate::less,
            DepthBufferAccess const access = DepthBufferAccess::read_write)
          : predicate_{predicate}
          , access_{access} {}

        void bind() const noexcept;

        static void unbind() noexcept;

      private:
        DepthPredicate predicate_;
        DepthBufferAccess access_;
    };
}  // namespace glpp
