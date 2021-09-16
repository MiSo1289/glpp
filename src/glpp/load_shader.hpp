#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <span>

#include "glpp/shader.hpp"

namespace glpp
{
    class ShaderFilesystem
    {
      public:
        [[nodiscard]] virtual auto exists(
            std::filesystem::path const& path) const
            -> bool
            = 0;

        [[nodiscard]] virtual auto canonical(
            std::filesystem::path const& path) const
            -> std::filesystem::path = 0;

        [[nodiscard]] virtual auto open(
            std::filesystem::path const& path) const
            -> std::unique_ptr<std::istream> = 0;

      protected:
        ~ShaderFilesystem() noexcept = default;
    };

    class DefaultShaderFilesystem : public ShaderFilesystem
    {
      public:
        [[nodiscard]] auto exists(
            std::filesystem::path const& path) const
            -> bool override
        {
            return std::filesystem::exists(path);
        }

        [[nodiscard]] auto canonical(
            std::filesystem::path const& path) const
            -> std::filesystem::path override
        {
            return std::filesystem::canonical(path);
        }

        [[nodiscard]] auto open(
            std::filesystem::path const& path) const
            -> std::unique_ptr<std::istream> override
        {
            return std::make_unique<std::ifstream>(path);
        }
    };

    // Compiles a shader from source files.
    // Sources can use non-standard #include "" directive;
    // includes are resolved from the include directories.
    // Nothing can appear inside a source file before #include
    // directives.
    // Multiple includes of the same file are ignored.
    // Sources should not declare GLSL version, instead the version
    // is passed as parameter to this function.
    //
    // Throws glpp::Error, glpp::ShaderCompilationError,
    // std::filesystem::filesystem_error
    [[nodiscard]] auto load_shader(
        ShaderType type,
        GlslVersion version,
        std::span<std::filesystem::path const> sources,
        std::span<std::filesystem::path const> include_directories = {},
        std::span<MacroDefinition const> definitions = {},
        ShaderFilesystem const& filesystem = DefaultShaderFilesystem{})
        -> Shader;
}  // namespace glpp