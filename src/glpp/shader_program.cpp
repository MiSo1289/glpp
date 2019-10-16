#include "glpp/shader_program.hpp"

#include <vector>

#include <gsl/gsl_util>

namespace glpp
{
    ShaderProgram::ShaderProgram(gsl::span<Shader const> shaders)
      : id_{glCreateProgram()}
    {
        link(shaders);
    }

    auto ShaderProgram::uniform_location(std::string const& name) const noexcept
        -> std::optional<UniformLocation>
    {
        if (auto location = glGetUniformLocation(id(), name.c_str()); location != -1)
            return UniformLocation{gsl::narrow<UInt32>(location)};
        return std::nullopt;
    }

    auto ShaderProgram::attribute_location(std::string const& name) const noexcept
        -> std::optional<AttributeLocation>
    {
        if (auto location = glGetAttribLocation(id(), name.c_str()); location != -1)
            return AttributeLocation{gsl::narrow<UInt32>(location)};
        return std::nullopt;
    }

    auto ShaderProgram::frag_output_location(std::string const& name) const noexcept
        -> std::optional<FragOutputLocation>
    {
        if (auto location = glGetFragDataLocation(id(), name.c_str()); location != -1)
            return FragOutputLocation{gsl::narrow<UInt32>(location)};
        return std::nullopt;
    }

    void ShaderProgram::Deleter::operator()(Id id) const noexcept
    {
        glDeleteProgram(id);
    }

    void ShaderProgram::link(gsl::span<Shader const> shaders)
    {
        for (auto const& shader : shaders)
            glAttachShader(id(), shader.id());

        glLinkProgram(id());

        for (auto const& shader : shaders)
            glDetachShader(id(), shader.id());

        auto success = Int32{};
        auto log_len = Int32{};
        glGetProgramiv(id(), GL_LINK_STATUS, &success);
        glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &log_len);

        if (!success)
        {
            auto log = std::vector<char>(static_cast<std::size_t>(log_len + 1));
            glGetProgramInfoLog(id(), log_len, nullptr, log.data());
            throw ShaderCompilationError{log.data()};
        }
    }
}  // namespace glpp