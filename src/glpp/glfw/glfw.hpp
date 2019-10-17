#pragma once

#include <atomic>
#include <functional>
#include <string>

#include "glpp/glfw/glfw_api.hpp"

namespace glpp::glfw
{
    class Glfw
    {
      public:
        [[nodiscard]] static auto get_instance() -> Glfw*
        {
            return instance;
        }

        // Only one instance can exist simultaneously.
        // If an instance does not exist, most glfw calls will fail.
        //
        // Throws glpp::InitError.
        Glfw();

        Glfw(Glfw const&) = delete;
        Glfw(Glfw&&) = delete;

        ~Glfw() noexcept;

        Glfw& operator=(Glfw const&) = delete;
        Glfw& operator=(Glfw&&) = delete;

        // Has to be called after a GLFW window has been created
        // and is the current context.
        // Called automatically when creating a glpp::glfw::Window.
        //
        // Throws glpp::InitError
        void load_gl();

      private:
        inline static auto instance = std::atomic<Glfw*>{nullptr};
    };
}  // namespace glpp::glfw