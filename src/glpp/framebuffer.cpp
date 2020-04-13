#include "glpp/framebuffer.hpp"

#include <utility>
#include <vector>

namespace
{
    thread_local auto default_framebuffer_viewport_size
        = std::optional<glpp::ViewportSize>{};
}

namespace glpp
{
    void Framebuffer::set_frag_output_textures(
        gsl::span<TextureAttachment const> const attachments)
    {
        auto const binding = ScopedBind{*this};

        auto const draw_buffers = [&]() -> std::vector<Enum> {
            if (attachments.empty())
            {
                return {};
            }

            auto const max_location = std::max_element(
                attachments.begin(),
                attachments.end(),
                [](auto const& lhs, auto const& rhs) {
                    return lhs.output_loc.value < rhs.output_loc.value;
                });
            auto draw_buffers = std::vector<Enum>(
                std::size_t{max_location->output_loc.value} + 1,
                GL_NONE);
            auto allocated_attachments = 0;

            for (auto const& [texture, output_loc] : attachments)
            {
                auto const attachment_name
                    = GL_COLOR_ATTACHMENT0 + allocated_attachments++;
                draw_buffers[output_loc.value] = attachment_name;
                glFramebufferTexture(
                    GL_FRAMEBUFFER,
                    attachment_name,
                    texture.id(),
                    0);
            }

            return draw_buffers;
        }();

        glDrawBuffers(
            static_cast<Size>(draw_buffers.size()),
            draw_buffers.data());
    }

    void Framebuffer::bind() const noexcept
    {
        if (!default_framebuffer_viewport_size.has_value())
        {
            default_framebuffer_viewport_size = get_viewport_size();
        }

        glpp::set_viewport_size(size_);
        glBindFramebuffer(GL_FRAMEBUFFER, id());
    }

    void Framebuffer::unbind() noexcept
    {
        glBindFramebuffer(GL_FRAMEBUFFER, nullid);
        if (default_framebuffer_viewport_size.has_value())
        {
            glpp::set_viewport_size(
                *std::exchange(default_framebuffer_viewport_size, std::nullopt));
        }
    }
}  // namespace glpp