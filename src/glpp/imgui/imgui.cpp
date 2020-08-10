#include "glpp/imgui/imgui.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace glpp::imgui
{
    ImGui::ImGui(glpp::glfw::Window& window)
      : window_{window}
    {
        ::IMGUI_CHECKVERSION();
        ::ImGui::CreateContext();
        ::ImGui_ImplGlfw_InitForOpenGL(window.api_ptr(), false);
        ::ImGui_ImplOpenGL3_Init();

        api_mouse_button_cb_conn_ = window.api_event_context().on_api_mouse_button([this](auto const... args) {
            ::ImGui_ImplGlfw_MouseButtonCallback(window_.api_ptr(), args...);
        });
        api_scroll_cb_conn_ = window.api_event_context().on_api_scroll([this](auto const... args) {
            ::ImGui_ImplGlfw_ScrollCallback(window_.api_ptr(), args...);
        });
        api_key_cb_conn_ = window.api_event_context().on_api_key([this](auto const... args) {
            ::ImGui_ImplGlfw_KeyCallback(window_.api_ptr(), args...);
        });
        api_char_cb_conn_ = window.api_event_context().on_api_char([this](auto const... args) {
            ::ImGui_ImplGlfw_CharCallback(window_.api_ptr(), args...);
        });
    }

    ImGui::~ImGui() noexcept
    {
        ::ImGui_ImplOpenGL3_Shutdown();
        ::ImGui_ImplGlfw_Shutdown();
        ::ImGui::DestroyContext();
    }

    void ImGui::new_frame()
    {
        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui_ImplGlfw_NewFrame();
        ::ImGui::NewFrame();
    }

    void ImGui::render()
    {
        ::ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
    }
}  // namespace glpp::imgui
