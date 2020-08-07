#include "glpp/shader.hpp"

#include <vector>

#include <gsl/gsl_util>

namespace glpp
{
    Shader::Shader(
        ShaderType const type,
        gsl::span<std::string const> const source_fragments)
      : id_{glCreateShader(static_cast<Enum>(type))}
    {
        if (!id())
        {
            throw glpp::Error{"Could not create a shader object"};
        }
        compile(source_fragments);
    }

    Shader::Shader(
        ShaderType const type,
        std::string const& source)
      : Shader{type, gsl::span{&source, 1}}
    {
    }

    void Shader::compile(gsl::span<std::string const> const source_fragments)
    {
        auto fragment_c_strings = std::vector<char const*>{};
        fragment_c_strings.reserve(source_fragments.size());
        std::transform(
            source_fragments.begin(),
            source_fragments.end(),
            std::back_inserter(fragment_c_strings),
            [](auto const& str) { return str.c_str(); });

        glShaderSource(
            id(),
            gsl::narrow<Size>(fragment_c_strings.size()),
            fragment_c_strings.data(),
            nullptr);
        glCompileShader(id());

        auto success = Int32{};
        auto log_len = Int32{};
        glGetShaderiv(id(), GL_COMPILE_STATUS, &success);
        glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &log_len);

        if (!success)
        {
            auto log = std::vector<char>(static_cast<std::size_t>(log_len) + 1);
            glGetShaderInfoLog(id(), log_len, nullptr, log.data());
            throw ShaderCompilationError{log.data()};
        }
    }
}  // namespace glpp