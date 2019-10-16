#pragma once

#include <glad/glad.h>

namespace glpp
{
    using Int8 = GLbyte;
    using Int16 = GLshort;
    using Int32 = GLint;
    using Int64 = GLint64;

    using UInt8 = GLubyte;
    using UInt16 = GLushort;
    using UInt32 = GLuint;
    using UInt64 = GLuint64;
    
	using Float32 = GLfloat;
    using Float64 = GLdouble;
    
	using Bool = GLboolean;
    using Enum = GLenum;
	using Size = GLsizei;
}  // namespace glpp
