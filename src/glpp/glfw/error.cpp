#include "glpp/glfw/error.hpp"

#include <cstdio>
#include <optional>
#include <stdexcept>
#include <utility>

#include <magic_enum.hpp>

namespace
{
    thread_local auto last_glfw_error = std::optional<glpp::glfw::GlfwError>{};

    extern "C" void glpp_glfw_error_callback(
        int const error,
        char const* const description) noexcept
    {
        try
        {
            last_glfw_error = glpp::glfw::GlfwError{
                magic_enum::enum_cast<glpp::glfw::ErrorCode>(error)
                    .value_or(glpp::glfw::ErrorCode::no_error),
                description,
            };
        }
        catch (std::exception const& error)
        {
            std::fprintf(
                stderr,
                "Uncaught exception when processing GLFW error event: %s\n",
                error.what());
        }
        catch (...)
        {
            std::fprintf(
                stderr,
                "Unknown uncaught exception when processing GLFW error event\n");
        }
    }
}  // namespace

namespace glpp::glfw
{
    void install_error_handler() noexcept
    {
        unchecked_api_invoke(&glfwSetErrorCallback, &glpp_glfw_error_callback);
    }

    void uninstall_error_handler() noexcept
    {
        unchecked_api_invoke(&glfwSetErrorCallback, nullptr);
    }

    void check_error()
    {
        if (auto error = std::exchange(last_glfw_error, std::nullopt))
        {
            throw std::move(*error);
        }
    }

    void clear_error() noexcept
    {
        last_glfw_error.reset();
    }
}  // namespace glpp::glfw