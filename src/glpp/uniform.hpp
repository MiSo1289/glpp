#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gsl/span>
#include "glpp/shader_program.hpp"

namespace glpp
{
    class UniformBase
    {
      public:
        explicit UniformBase(UniformLocation const location) noexcept
          : location_(location) {}

        [[nodiscard]] auto location() const noexcept -> UniformLocation { return location_; }

      private:
        UniformLocation location_;
    };

    struct SamplerUnit
    {
        UInt32 index = 0;
    };

    class TextureSamplerUniform : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(SamplerUnit const sampler_unit) const noexcept
        {
            glUniform1i(location().value, sampler_unit.index);
        }
    };

    template <typename T>
    class ArrayUniform;

    template <typename T>
    class Uniform
    {
      public:
        explicit Uniform(UniformLocation const location) noexcept
          : impl_{location}
        {
        }

        [[nodiscard]] auto location() const noexcept -> UniformLocation
        {
            return impl_.location();
        }

        void load(T const value) noexcept
        {
            impl_.load(gsl::span{&value, 1});
        }

      private:
        ArrayUniform<T> impl_;
    };

    template <>
    class ArrayUniform<Float32> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<Float32 const> const buffer) noexcept
        {
            glUniform1fv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    template <>
    class ArrayUniform<Int32> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<Int32 const> const buffer) noexcept
        {
            glUniform1iv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    template <>
    class ArrayUniform<UInt32> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<UInt32 const> const buffer) noexcept
        {
            glUniform1uiv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    template <>
    class ArrayUniform<glm::mat2> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::mat2 const> const buffer) noexcept
        {
            glUniformMatrix2fv(location().value, static_cast<GLsizei>(buffer.size()), false, glm::value_ptr(buffer[0]));
        }
    };

    template <>
    class ArrayUniform<glm::mat3> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::mat3 const> const buffer) noexcept
        {
            glUniformMatrix3fv(location().value, static_cast<GLsizei>(buffer.size()), false, glm::value_ptr(buffer[0]));
        }
    };

    template <>
    class ArrayUniform<glm::mat4> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::mat4 const> const buffer) noexcept
        {
            glUniformMatrix4fv(location().value, static_cast<GLsizei>(buffer.size()), false, glm::value_ptr(buffer[0]));
        }
    };

    template <>
    class ArrayUniform<glm::vec2> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::vec2 const> const buffer) noexcept
        {
            glUniform2fv(location().value, static_cast<GLsizei>(buffer.size()), glm::value_ptr(buffer[0]));
        }
    };

    template <>
    class ArrayUniform<glm::vec3> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::vec3 const> const buffer) noexcept
        {
            glUniform3fv(location().value, static_cast<GLsizei>(buffer.size()), glm::value_ptr(buffer[0]));
        }
    };

    template <>
    class ArrayUniform<glm::vec4> : public UniformBase
    {
      public:
        using UniformBase::UniformBase;

        void load(gsl::span<glm::vec4 const> const buffer) noexcept
        {
            glUniform4fv(location().value, static_cast<GLsizei>(buffer.size()), glm::value_ptr(buffer[0]));
        }
    };
}  // namespace glpp
