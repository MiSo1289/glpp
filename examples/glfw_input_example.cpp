#include <iostream>
#include <stdexcept>
#include <thread>

#include <boost/signals2.hpp>
#include <clara.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <glpp/glfw/glfw.hpp>
#include <glpp/glfw/window.hpp>

auto main(int const argc, char const* const* const argv) noexcept -> int
{
    using namespace std::chrono_literals;

    try
    {
        auto show_help = false;
        auto log_all = false;
        auto log_key_events = false;
        auto log_mouse_button_events = false;
        auto log_mouse_move_events = false;
        auto log_mouse_scroll_events = false;

        auto const cli
            = clara::Help(show_help)
              | clara::Opt{log_all}
                  ["-a"]["--all"]("Log all events (equivalent to -kbms)")
              | clara::Opt{log_key_events}
                  ["-k"]["--keys"]("Log key events")
              | clara::Opt{log_mouse_button_events}
                  ["-b"]["--mouse-buttons"]("Log mouse button events")
              | clara::Opt{log_mouse_move_events}
                  ["-m"]["--mouse-move"]("Log mouse move events")
              | clara::Opt{log_mouse_scroll_events}
                  ["-s"]["--mouse-scroll"]("Log mouse scroll events");

        if (auto const parse_result = cli.parse(clara::Args(argc, argv));
            !parse_result)
        {
            std::cerr << parse_result.errorMessage() << "\n"
                      << "Use --help to see usage\n";
            return 1;
        }

        if (show_help)
        {
            std::cout << cli;
            return 0;
        }

        if (log_all)
        {
            log_key_events = true;
            log_mouse_button_events = true;
            log_mouse_move_events = true;
            log_mouse_scroll_events = true;
        }

        auto glfw = glpp::glfw::Glfw{};
        auto window = glpp::glfw::Window{
            glfw,
            glpp::glfw::WindowMode{
                glpp::glfw::WindowType::windowed,
                // The holy resolution
                640,
                480,
            },
            "GLFW Input example",
        };

        auto key_cb_conn = boost::signals2::scoped_connection{};
        auto mouse_button_cb_conn = boost::signals2::scoped_connection{};
        auto mouse_move_cb_conn = boost::signals2::scoped_connection{};
        auto mouse_scroll_cb_conn = boost::signals2::scoped_connection{};

        if (log_key_events)
        {
            key_cb_conn = window.on_key([](auto const event) {
                std::cout << fmt::format(
                    "Key {0} ({1}); action {2}; mods {3}\n",
                    event.scan_code,
                    event.key,
                    event.action,
                    event.mods);
            });
        }

        if (log_mouse_button_events)
        {
            mouse_button_cb_conn = window.on_mouse_button([](auto const event) {
                std::cout << fmt::format(
                    "Mouse button {0} ({1}); action {2}; mods {3}\n",
                    static_cast<int>(event.button),
                    event.button,
                    event.action,
                    event.mods);
            });
        }

        if (log_mouse_move_events)
        {
            mouse_move_cb_conn = window.on_cursor_pos([](auto const event) {
                std::cout << fmt::format(
                    "Mouse position {0} {1}\n",
                    event.xpos,
                    event.ypos);
            });
        }

        if (log_mouse_scroll_events)
        {
            mouse_scroll_cb_conn = window.on_scroll([](auto const event) {
                std::cout << fmt::format(
                    "Mouse scroll offset {0} {1}\n",
                    event.xoff,
                    event.yoff);
            });
        }

        while (window.is_open())
        {
            std::this_thread::sleep_for(10ms);
            window.poll_events();
            window.swap_buffers();
        }
    }
    catch (std::exception const& error)
    {
        std::cerr << error.what() << "\n";
        return 2;
    }

    return 0;
}
