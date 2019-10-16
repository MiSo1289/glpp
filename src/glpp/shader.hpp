#pragma once

#include <filesystem>
#include <string>

#include <glad/glad.h>
#include <gsl/span>
#include "glpp/error.hpp"
#include "glpp/id.hpp"

namespace glpp
{
    enum class ShaderType : Enum
    {
        compute_shader = GL_COMPUTE_SHADER,
        vertex_shader = GL_VERTEX_SHADER,
        tess_control_shader = GL_TESS_CONTROL_SHADER,
        tess_evaluation_shader = GL_TESS_EVALUATION_SHADER,
        geometry_shader = GL_GEOMETRY_SHADER,
        fragment_shader = GL_FRAGMENT_SHADER,
    };

    enum class GlslVersionNumber : std::uint16_t
    {
        glsl_110 = 110,
        glsl_120 = 120,
        glsl_130 = 130,
        glsl_140 = 140,
        glsl_150 = 150,
        glsl_330 = 330,
        glsl_400 = 400,
        glsl_410 = 410,
        glsl_420 = 420,
        glsl_430 = 430,
    };

    enum class GlslProfile : std::uint8_t
    {
        core,
        compatibility,
    };

    struct GlslVersion
    {
        GlslVersionNumber version_number = GlslVersionNumber::glsl_430;
        GlslProfile profile = GlslProfile::core;
    };

    struct MacroDefinition
    {
        std::string name;
        std::string value;
    };

    class Shader
    {
      public:
        // Compiles a shader from sources.
        // Sources can use non-standard #include "" directive;
        // includes are resolved from the include directories.
        // Nothing can appear inside a source file before #include
        // directives.
		// Multiple includes of the same file are ignored.
        // Sources should not declare GLSL version, instead the version
        // is passed as parameter to this function.
        //
        // Throws glpp::Error, glpp::ShaderCompilationError
        Shader(
            ShaderType type,
            GlslVersion version,
            gsl::span<std::filesystem::path const> sources,
            gsl::span<std::filesystem::path const> include_directories,
            gsl::span<MacroDefinition const> definitions);

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

      private:
        struct Deleter
        {
            void operator()(Id id) const noexcept
            {
                glDeleteShader(id);
            }
        };

        using ShaderId = UniqueId<Deleter>;

        ShaderId id_;

        void compile(
            gsl::span<std::string const> sourceFragments);
    };

}  // namespace glpp
