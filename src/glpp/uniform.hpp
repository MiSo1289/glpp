#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gsl/span>
#include "glpp/shader_program.hpp"

namespace glpp
{
    class Uniform
    {
      public:
        explicit Uniform(UniformLocation location) noexcept
          : location_(location) {}

        [[nodiscard]] auto location() const noexcept -> UniformLocation { return location_; }

      private:
        UniformLocation location_;
    };

    struct SamplerUnit
    {
        UInt32 index;
    };

    class TextureSamplerUniform : public Uniform
    {
      public:
        TextureSamplerUniform(UniformLocation location, SamplerUnit sampler_unit) noexcept
          : Uniform(location), sampler_unit_(sampler_unit) {}

        void bind() const noexcept
        {
            glUniform1i(location().value, sampler_unit_.index);
            glActiveTexture(GL_TEXTURE0 + sampler_unit_.index);
        }

        void unbind() const noexcept
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(location().value, 0);
        }

      private:
        SamplerUnit sampler_unit_;
    };

    class Mat2Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::mat2 mat) noexcept
        {
            glUniformMatrix2fv(location().value, 1, false, glm::value_ptr(mat));
        }
    };

    class Mat3Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::mat3 mat) noexcept
        {
            glUniformMatrix3fv(location().value, 1, false, glm::value_ptr(mat));
        }
    };

    class Mat4Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::mat4 mat) noexcept
        {
            glUniformMatrix4fv(location().value, 1, false, glm::value_ptr(mat));
        }
    };

    class Vec2Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::vec2 vec) noexcept
        {
            glUniform2fv(location().value, 1, glm::value_ptr(vec));
        }
    };

    class Vec3Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::vec3 vec) noexcept
        {
            glUniform3fv(location().value, 1, glm::value_ptr(vec));
        }
    };

    class Vec4Uniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(glm::vec4 vec) noexcept
        {
            glUniform4fv(location().value, 1, glm::value_ptr(vec));
        }
    };

    template <typename T, std::size_t size>
    class ArrayUniform;

    template <std::size_t size>
    class ArrayUniform<Float32, size> : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(gsl::span<Float32 const> buffer) noexcept
        {
            assert(buffer.size() <= size);
            glUniform1fv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    template <std::size_t size>
    class ArrayUniform<Int32, size> : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(gsl::span<Int32 const> buffer) noexcept
        {
            assert(buffer.size() <= size);
            glUniform1iv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    template <std::size_t size>
    class ArrayUniform<UInt32, size> : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(gsl::span<UInt32 const> buffer) noexcept
        {
            assert(buffer.size() <= size);
            glUniform1uiv(location().value, static_cast<GLsizei>(buffer.size()), buffer.data());
        }
    };

    class FloatUniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(Float32 value) noexcept
        {
            glUniform1fv(location().value, 1, &value);
        }
    };

    class UIntUniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(UInt32 value) noexcept
        {
            glUniform1uiv(location().value, 1, &value);
        }
    };

    class IntUniform : public Uniform
    {
      public:
        using Uniform::Uniform;

        void load(Int32 value) noexcept
        {
            glUniform1iv(location().value, 1, &value);
        }
    };
}  // namespace glpp
