#include "glpp/config/config.hpp"

#include <algorithm>
#include <string>

#include <magic_enum.hpp>
#include "glpp/config/error.hpp"

namespace glpp::config
{
    void to_json(nlohmann::json& json, ShaderConfig const& shader_config)
    {
        auto sources = nlohmann::json::array();
        for (auto const& source_path : shader_config.sources)
        {
            sources.push_back(source_path.string());
        }

        json = {
            {"shaderType", magic_enum::enum_name(shader_config.shader_type)},
            {"sources", std::move(sources)},
        };
    }

    void from_json(nlohmann::json const& json, ShaderConfig& shader_config)
    {
        if (auto shader_type
            = magic_enum::enum_cast<ShaderType>(json.at("shaderType").get<std::string>()))
        {
            shader_config.shader_type = *shader_type;
            shader_config.sources.clear();
            for (auto const& source : json.at("sources"))
            {
                shader_config.sources.push_back(source.get<std::string>());
            }
        }
        else
        {
            throw ConfigError{"Bad shader type"};
        }
    }

    void to_json(nlohmann::json& json, ShaderProgramConfig const& shader_program_config)
    {
        auto definitions = nlohmann::json::array();
        for (auto const& def : shader_program_config.definitions)
        {
            definitions.push_back({
                {"name", def.name},
                {"value", def.value},
            });
        }

        auto include_directories = nlohmann::json::array();
        for (auto const& include_dir : shader_program_config.include_directories)
        {
            include_directories.push_back(include_dir.string());
        }

        json = {
            {"glslVersion",
             {
                 {
                     "versionNumber",
                     magic_enum::enum_integer(shader_program_config.glsl_version.version_number),
                 },
                 {
                     "profile",
                     magic_enum::enum_name(shader_program_config.glsl_version.profile),
                 },
             }},
            {"shaders", shader_program_config.shaders},
            {"definitions", std::move(definitions)},
            {"includeDirectories", std::move(include_directories)},

        };
    }

    void from_json(nlohmann::json const& json, ShaderProgramConfig& shader_program_config)
    {
        auto const& version = json.at("glslVersion");
        if (auto version_number
            = magic_enum::enum_cast<GlslVersionNumber>(version.at("versionNumber").get<int>()))
        {
            shader_program_config.glsl_version.version_number = *version_number;
            if (auto profile
                = magic_enum::enum_cast<GlslProfile>(version.at("profile").get<std::string>()))
            {
                shader_program_config.glsl_version.profile = *profile;

                json.at("shaders").get_to(shader_program_config.shaders);

                shader_program_config.definitions.clear();
                for (auto const& definition_json : json.at("definitions"))
                {
                    auto definition = MacroDefinition{};
                    definition_json.at("name").get_to(definition.name);
                    definition_json.at("value").get_to(definition.value);
                    shader_program_config.definitions.push_back(std::move(definition));
                }

                shader_program_config.include_directories.clear();
                for (auto const& include_dir : json.at("includeDirectories"))
                {
                    shader_program_config.include_directories.push_back(include_dir.get<std::string>());
                }
            }
            else
            {
                throw ConfigError{"Bad GLSL profile"};
            }
        }
        else
        {
            throw ConfigError{"Bad GLSL version number"};
        }
    }

    auto make_shader(
        ShaderConfig const& config,
        GlslVersion version,
        std::vector<MacroDefinition> definitions,
        std::vector<std::filesystem::path> include_directories)
        -> Shader
    {
        return Shader{
            config.shader_type,
            version,
            config.sources,
            include_directories,
            definitions,
        };
    }

    [[nodiscard]] auto make_shader_program(ShaderProgramConfig const& config) -> ShaderProgram
    {
        auto shaders = std::vector<Shader>{};

        std::transform(
            config.shaders.begin(),
            config.shaders.end(),
            std::back_inserter(shaders),
            [&](auto const& shader_config) {
                return make_shader(
                    shader_config,
                    config.glsl_version,
                    config.definitions,
                    config.include_directories);
            });

        return ShaderProgram{shaders};
    }
}  // namespace glpp::config