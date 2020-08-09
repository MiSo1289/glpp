#pragma once

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

        void new_frame();

        void render();

      private:
        glfw::Window& window_;
        boost::signals2::scoped_connection mouse_button_cb_conn_;
        boost::signals2::scoped_connection scroll_cb_conn_;
        boost::signals2::scoped_connection key_cb_conn_;
        boost::signals2::scoped_connection char_cb_conn_;
    };
}  // namespace glpp::imgui
