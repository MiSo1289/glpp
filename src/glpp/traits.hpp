#pragma once

#include <glad/glad.h>
#include "glpp/primitive_types.hpp"

namespace glpp
{
    template <typename PrimitiveType>
    struct PrimitiveTypeTraits
    {
    };

    template <>
    struct PrimitiveTypeTraits<Float32>
    {
        static constexpr Enum enumerator = GL_FLOAT;
    };

    template <>
    struct PrimitiveTypeTraits<Float64>
    {
        static constexpr Enum enumerator = GL_DOUBLE;
    };

    template <>
    struct PrimitiveTypeTraits<Int8>
    {
        static constexpr Enum enumerator = GL_BYTE;
    };

    template <>
    struct PrimitiveTypeTraits<Int16>
    {
        static constexpr Enum enumerator = GL_SHORT;
    };

    template <>
    struct PrimitiveTypeTraits<Int32>
    {
        static constexpr Enum enumerator = GL_INT;
    };

    template <>
    struct PrimitiveTypeTraits<UInt8>
    {
        static constexpr Enum enumerator = GL_UNSIGNED_BYTE;
    };

    template <>
    struct PrimitiveTypeTraits<UInt16>
    {
        static constexpr Enum enumerator = GL_UNSIGNED_SHORT;
    };

    template <>
    struct PrimitiveTypeTraits<UInt32>
    {
        static constexpr Enum enumerator = GL_UNSIGNED_INT;
    };

    template <typename PrimitiveType>
    inline constexpr auto primitive_type_enumerator_v = PrimitiveTypeTraits<PrimitiveType>::enumerator;
}  // namespace glpp
