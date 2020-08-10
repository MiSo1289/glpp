#include <chrono>
#include <iostream>
#include <thread>

#include <glpp/draw.hpp>
#include <glpp/glfw/glfw.hpp>
#include <glpp/glfw/window.hpp>
#include <glpp/imgui/imgui.hpp>

auto main() noexcept -> int
{
    using namespace std::chrono_literals;

    try
    {
        // Create the window
        auto glfw = glpp::glfw::Glfw{};
        auto window = glpp::glfw::Window{
            glfw,
            glpp::glfw::WindowMode{
                glpp::glfw::WindowType::windowed,
                // The holy resolution
                640,
                480,
            },
            "Draw example",
        };

        // Create GUI
        auto gui = glpp::imgui::ImGui{window};
        ImGui::StyleColorsLight();
        //ImGuiStyle::ScaleAllSizes(5.0f);
        float SCALE = 5.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * SCALE;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg)->DisplayOffset.y = SCALE;

        while (window.is_open())
        {
            glpp::clear_color({0.0f, 0.0f, 0.0f, 1.0f});
            window.poll_events();
            gui.new_frame();

            ImGui::Begin("Demo window");
            ImGui::Button("Hello!");
            ImGui::End();

            gui.render();
            window.swap_buffers();
            std::this_thread::sleep_for(10ms);
        }
    }
    catch (std::exception const& error)
    {
        std::cerr << error.what() << "\n";
        return 2;
    }

    return 0;
}