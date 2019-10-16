#pragma once

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>
#include "glpp/shader.hpp"
#include "glpp/shader_program.hpp"

namespace glpp::config
{
    struct ShaderConfig
    {
        ShaderType shader_type;
        std::vector<std::filesystem::path> sources;
    };

    void to_json(nlohmann::json& json, ShaderConfig const& shader_config);
    void from_json(nlohmann::json const& json, ShaderConfig& shader_config);

    struct ShaderProgramConfig
    {
        GlslVersion glsl_version;
        std::vector<ShaderConfig> shaders;
        std::vector<MacroDefinition> definitions;
        std::vector<std::filesystem::path> include_directories;
    };

    void to_json(nlohmann::json& json, ShaderProgramConfig const& shader_program_config);
    void from_json(nlohmann::json const& json, ShaderProgramConfig& shader_program_config);

    [[nodiscard]] auto make_shader(
        ShaderConfig const& config,
        GlslVersion version,
        std::vector<MacroDefinition> definitions,
        std::vector<std::filesystem::path> include_directories)
        -> Shader;

	[[nodiscard]] auto make_shader_program(ShaderProgramConfig const& config) -> ShaderProgram;

}  // namespace glpp::config