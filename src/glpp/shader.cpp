#include "glpp/shader.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <magic_enum.hpp>

namespace
{
    [[nodiscard]] auto define_directive(glpp::MacroDefinition const& definition) -> std::string
    {
        return fmt::format(
            "#define {0} {1}\n",
            definition.name,
            definition.value);
    }

    [[nodiscard]] auto version_directive(glpp::GlslVersion version) -> std::string
    {
        return fmt::format(
            "#version {0} {1}\n",
            magic_enum::enum_integer(version.version_number),
            magic_enum::enum_name(version.profile));
    }

    [[nodiscard]] auto prelude_fragment(
        glpp::GlslVersion version,
        gsl::span<glpp::MacroDefinition const> definitions)
        -> std::string
    {
        auto prelude_stream = std::ostringstream{};
        prelude_stream << version_directive(version);
        std::transform(
            definitions.begin(),
            definitions.end(),
            std::ostream_iterator<std::string>{prelude_stream},
            &define_directive);

        return prelude_stream.str();
    }

    [[nodiscard]] auto find_include(
        std::filesystem::path const& include,
        gsl::span<std::filesystem::path const> include_directories)
        -> std::optional<std::filesystem::path>
    {
        if (std::filesystem::exists(include))
        {
            return include;
        }

        for (auto const& include_dir : include_directories)
        {
            auto full_path = include_dir / include;
            if (std::filesystem::exists(full_path))
            {
                return full_path;
            }
        }

        return std::nullopt;
    }

    void resolve_includes(
        std::filesystem::path const& source,
        gsl::span<std::filesystem::path const> include_directories,
        std::vector<std::string>& source_fragments,
        std::unordered_set<std::string>& open_files,
        std::unordered_set<std::string>& resolved_files)
    {
        auto source_canonical = std::filesystem::canonical(source).string();
        if (resolved_files.find(source_canonical) != resolved_files.end())
        {
            return;
        }

        if (open_files.find(source_canonical) != open_files.end())
        {
            throw glpp::ShaderCompilationError{
                fmt::format("Cyclical dependency to file '{0}'", source),
            };
        }
        open_files.insert(source_canonical);

        auto file = std::ifstream{source};
        if (!file.is_open())
        {
            throw glpp::ShaderCompilationError{
                fmt::format("Failed to open shader source '{0}'", source),
            };
        }

        auto line = std::string{};
        auto token = std::string{};
        auto fragment = std::string{};

        bool prologue_end = false;
        int line_no = 1;

        while (std::getline(file, line))
        {
            auto line_stream = std::istringstream{line};
            if (line_stream >> token)
            {
                if (token == "#include")
                {
                    if (prologue_end)
                    {
                        throw glpp::ShaderCompilationError{
                            fmt::format(
                                "In file '{0}' at line {1}: "
                                "Invalid #include directive after non-empty line",
                                source,
                                line_no),
                        };
                    }

                    if (line_stream >> token
                        && token.front() == '"'
                        && token.back() == '"')
                    {
                        auto include_file = std::filesystem::path{
                            token.substr(1, token.size() - 2),
                        };
                        if (auto include_full_path
                            = find_include(include_file, include_directories))
                        {
                            resolve_includes(
                                *include_full_path,
                                include_directories,
                                source_fragments,
                                open_files,
                                resolved_files);
                            line.clear();
                        }
                        else
                        {
                            throw glpp::ShaderCompilationError{
                                fmt::format(
                                    "In file '{0}' at line {1}: "
                                    "Failed to open shader include '{2}'",
                                    source,
                                    line_no,
                                    include_file),
                            };
                        }
                    }
                    else
                    {
                        throw glpp::ShaderCompilationError{
                            fmt::format(
                                "In file '{0}' at line {1}: "
                                "Invalid #include directive: '{2}'",
                                source,
                                line_no,
                                line),
                        };
                    }
                }
                else
                {
                    prologue_end = true;
                }
            }

            fragment += line;
            fragment += "\n";
            line_no++;
        }

        source_fragments.push_back(fragment);
        resolved_files.insert(source_canonical);
    }

    [[nodiscard]] auto resolve_source_fragments(
        glpp::GlslVersion version,
        gsl::span<std::filesystem::path const> sources,
        gsl::span<std::filesystem::path const> include_directories = {},
        gsl::span<glpp::MacroDefinition const> definitions = {})
        -> std::vector<std::string>
    {
        auto source_fragments = std::vector<std::string>{};
        source_fragments.push_back(
            prelude_fragment(version, definitions));

        auto open_files = std::unordered_set<std::string>{};
        auto resolved_files = std::unordered_set<std::string>{};

        for (auto const& source : sources)
        {
            resolve_includes(
                source,
                include_directories,
                source_fragments,
                open_files,
                resolved_files);
        }

        return source_fragments;
    }
}  // namespace

namespace glpp
{
    Shader::Shader(
        ShaderType type,
        GlslVersion version,
        gsl::span<std::filesystem::path const> sources,
        gsl::span<std::filesystem::path const> include_directories,
        gsl::span<MacroDefinition const> definitions)
      : id_{glCreateShader(static_cast<Enum>(type))}
    {
        if (!id())
        {
            throw Error{"Could not create shader object"};
        }

        auto const fragments = resolve_source_fragments(
            version,
            sources,
            include_directories,
            definitions);

        compile(fragments);
    }

    void Shader::compile(gsl::span<std::string const> source_fragments)
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
            auto log = std::vector<char>(static_cast<std::size_t>(log_len + 1));
            glGetShaderInfoLog(id(), log_len, nullptr, log.data());
            throw ShaderCompilationError{std::string{log.data()}};
        }
    }
}  // namespace glpp