#pragma once

#include <optional>
#include <string>

#include <glad/glad.h>
#include <gsl/span>
#include "glpp/id.hpp"
#include "glpp/primitive_types.hpp"
#include "glpp/shader.hpp"

namespace glpp
{
    struct AttributeLocation
    {
        UInt32 value;
    };

    struct FragOutputLocation
    {
        UInt32 value;
    };

    struct UniformLocation
    {
        UInt32 value;
    };

    class ShaderProgram
    {
      public:
        explicit ShaderProgram(gsl::span<Shader const> shaders);

        void bind() const noexcept
        {
            glUseProgram(id());
        }

        static void unbind() noexcept
        {
            glUseProgram(nullid);
        }

        [[nodiscard]] auto uniform_location(std::string const& name) const noexcept
            -> std::optional<UniformLocation>;

        [[nodiscard]] auto attribute_location(std::string const& name) const noexcept
            -> std::optional<AttributeLocation>;

        [[nodiscard]] auto frag_output_location(std::string const& name) const noexcept
            -> std::optional<FragOutputLocation>;

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

      private:
        struct Deleter
        {
            void operator()(Id id) const noexcept;
        };

        UniqueId<Deleter> id_;

        void link(gsl::span<Shader const> shaders);
    };

}  // namespace glpp