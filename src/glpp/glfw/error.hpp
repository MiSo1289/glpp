#pragma once

#include <type_traits>

#include "glpp/error.hpp"
#include "glpp/glfw/glfw.hpp"

namespace glpp::glfw
{
    enum class ErrorCode
    {
        no_error = GLFW_NO_ERROR,
        not_initialized = GLFW_NOT_INITIALIZED,
        no_current_context = GLFW_NO_CURRENT_CONTEXT,
        invalid_enum = GLFW_INVALID_ENUM,
        invalid_value = GLFW_INVALID_VALUE,
        out_of_memory = GLFW_OUT_OF_MEMORY,
        api_unavailable = GLFW_API_UNAVAILABLE,
        version_unavailable = GLFW_VERSION_UNAVAILABLE,
        platform_error = GLFW_PLATFORM_ERROR,
        format_unavailable = GLFW_FORMAT_UNAVAILABLE,
        no_window_context = GLFW_NO_WINDOW_CONTEXT,
    };

    class GlfwError : public Error
    {
      public:
        GlfwError(
            ErrorCode const error_code,
            char const* const msg)
          : Error{msg}
          , error_code_{error_code} {}

        [[nodiscard]] auto error_code() const noexcept -> ErrorCode
        {
            return error_code_;
        }

      private:
        ErrorCode error_code_;
    };

    // Called by Glfw::Glfw()
    void install_error_handler() noexcept;

    // Called by Glfw::~Glfw()
    void uninstall_error_handler() noexcept;

    // Called by this library after most GLFW calls
    //
    // Throws glpp::glfw::GlfwError
    void check_error();

    // Called by this library after GLFW calls in desctructors
    // (to prevent further successfull calls from throwing in case
	// destruction fails)
	//
    // Throws glpp::glfw::GlfwError
    void clear_error() noexcept;

    template <typename... Args, typename... PassedArgs>
    void checked_api_invoke(
        void (*api_fn)(Args...),
        PassedArgs const... args)
    {
        static_assert((std::is_convertible_v<PassedArgs, Args> && ...));

        api_fn(args...);
        check_error();
    }

    template <typename Ret, typename... Args, typename... PassedArgs>
    auto checked_api_invoke(
        Ret (*api_fn)(Args...),
        PassedArgs&&... args) -> Ret
    {
        static_assert((std::is_convertible_v<PassedArgs&&, Args> && ...));

        auto result = api_fn(std::forward<PassedArgs>(args)...);
        check_error();

        return result;
    }

    template <typename... Args, typename... PassedArgs>
    void unchecked_api_invoke(
        void (*api_fn)(Args...),
        PassedArgs&&... args)
    {
        static_assert((std::is_convertible_v<PassedArgs&&, Args> && ...));

        api_fn(std::forward<PassedArgs>(args)...);
        clear_error();
    }

    template <typename Ret, typename... Args, typename... PassedArgs>
    auto unchecked_api_invoke(
        Ret (*api_fn)(Args...),
        PassedArgs&&... args) -> Ret
    {
        static_assert((std::is_convertible_v<PassedArgs&&, Args> && ...));

        auto result = api_fn(std::forward<PassedArgs>(args)...);
        clear_error();

        return result;
    }
}  // namespace glpp::glfw
