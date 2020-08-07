#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "glpp/buffer.hpp"
#include "glpp/config/config.hpp"
#include "glpp/draw.hpp"
#include "glpp/glfw/glfw.hpp"
#include "glpp/glfw/window.hpp"
#include "glpp/scoped_bind.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/uniform.hpp"
#include "glpp/vertex_array.hpp"

namespace
{
    auto const shader_cfg_path = std::filesystem::path{
        "examples/shaders/draw_example_program.json",
    };
}  // namespace

auto main() noexcept -> int
{
    try
    {
        // Load the shader program config file
        auto shader_cfg_file = std::ifstream{shader_cfg_path};
        if (!shader_cfg_file.is_open())
        {
            std::cerr << "Could not open shader config.\n"
                         "Run this example from the project root.\n";
            return 1;
        }
        auto shader_cfg_json = nlohmann::json();
        shader_cfg_file >> shader_cfg_json;

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

        // Create GL structures
        auto shader_program = glpp::config::make_shader_program(shader_cfg_json);
        auto vertex_pos_buffer = glpp::StaticAttribBuffer<float>{};
        auto vertex_array = glpp::VertexArray{};
        vertex_array.bind_attribute_buffer(
            vertex_pos_buffer.view(),
            glpp::AttributeLocation{0},
            3);
        auto color_uniform = glpp::Uniform<glm::vec4>{
            glpp::UniformLocation{0},
        };

        // Load vertex positions
        vertex_pos_buffer.buffer_data(std::array{
            -0.5f,
            -0.5f,
            -1.0f,

            0.5f,
            -0.5f,
            -1.0f,

            0.0f,
            0.5f,
            -1.0f,
        });

        // Bind the shader before loading a uniform
        auto shader_binding = glpp::ScopedBind{shader_program};
        // Set the color
        color_uniform.load({1.0f, 0.0f, 0.0f, 1.0f});

        // Bind the vertex array object for drawing
        auto vao_binding = glpp::ScopedBind{vertex_array};

        auto const draw_scene = [&]() {
            glpp::clear_color({0.0f, 0.0f, 0.0f, 1.0f});
            glpp::draw(glpp::DrawPrimitive::triangles, 3);
            window.swap_buffers();
        };

        draw_scene();

        auto const redraw_on_resize_conn
            = window.on_framebuffer_size([&]([[maybe_unused]] auto const event) {
                  draw_scene();
              });

        while (window.is_open())
        {
            using namespace std::chrono_literals;

            std::this_thread::sleep_for(10ms);
            window.poll_events();
        }
    }
    catch (std::exception const& error)
    {
        std::cerr << error.what() << "\n";
        return 2;
    }

    return 0;
}