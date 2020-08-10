#pragma once

#include <vector>

#include <boost/signals2.hpp>
#include <imgui.h>
#include "glpp/glfw/window.hpp"

namespace glpp::imgui
{
    class ImGui
    {
      public:
        explicit ImGui(glfw::Window& window);

        ImGui(ImGui const&) = delete;
        ImGui(ImGui&&) = delete;

        ~ImGui() noexcept;

        auto operator=(ImGui const&) -> ImGui& = delete;
        auto operator=(ImGui&&) -> ImGui& = delete;

        [[nodiscard]] auto window() noexcept
            -> glfw::Window& { return window_; }

        [[nodiscard]] auto window() const noexcept
            -> glfw::Window const& { return window_; }

        void new_frame();

        void render();

      private:
        glfw::Window& window_;
        boost::signals2::scoped_connection api_mouse_button_cb_conn_;
        boost::signals2::scoped_connection api_scroll_cb_conn_;
        boost::signals2::scoped_connection api_key_cb_conn_;
        boost::signals2::scoped_connection api_char_cb_conn_;
    };
}  // namespace glpp::imgui
