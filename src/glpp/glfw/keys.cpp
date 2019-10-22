#include "glpp/glfw/keys.hpp"

#include <magic_enum.hpp>

namespace magic_enum
{
    template <>
    struct enum_range<glpp::glfw::KeyCode>
    {
        static constexpr int min = GLFW_KEY_UNKNOWN;
        static constexpr int max = GLFW_KEY_LAST;
    };
}  // namespace magic_enum

namespace glpp::glfw
{
    auto to_string(KeyAction const action) noexcept
        -> std::string_view
    {
        return magic_enum::enum_name(action);
    }

    auto key_action_from_string(std::string_view const string) noexcept
        -> std::optional<KeyAction>
    {
        return magic_enum::enum_cast<KeyAction>(string);
    }

    auto operator<<(std::ostream& out, KeyAction const action) -> std::ostream&
    {
        return out << to_string(action);
    }

    auto operator<<(std::ostream& out, KeyMod const mod) -> std::ostream&
    {
        out << "{";
        auto sep = "";
        
		for (auto const value : magic_enum::enum_values<KeyMod>())
        {
            if ((mod & value) != KeyMod::none)
            {
                out << sep << magic_enum::enum_name(value);
                sep = ", ";
            }
        }
        
		return out << "}";
    }

    auto to_string(KeyCode const key) noexcept
        -> std::string_view
    {
        return magic_enum::enum_name(key);
    }

    auto key_code_from_string(std::string_view const string) noexcept
        -> std::optional<KeyCode>
    {
        return magic_enum::enum_cast<KeyCode>(string);
    }

    auto operator<<(std::ostream& out, KeyCode const key) -> std::ostream&
    {
        return out << to_string(key);
    }

    auto to_string(MouseButton const button) noexcept
        -> std::string_view
    {
        return magic_enum::enum_name(button);
    }

    auto mouse_button_from_string(std::string_view const string) noexcept
        -> std::optional<MouseButton>
    {
        return magic_enum::enum_cast<MouseButton>(string);
    }

    auto operator<<(std::ostream& out, MouseButton const button) -> std::ostream&
    {
        return out << to_string(button);
    }
}  // namespace glpp::glfw