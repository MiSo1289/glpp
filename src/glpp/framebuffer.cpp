#include "glpp/framebuffer.hpp"

#include <cassert>

namespace glpp
{
    void Framebuffer::set_frag_output_textures(
        gsl::span<TextureAttachment const> attachments) noexcept
    {
        assert(attachments.size() > 0);

        auto fb_bind = ScopedBind{*this};

        auto allocated_attachments = 0;
        auto max_location = std::max(
            attachments.begin(),
            attachments.end(),
            [](auto lhs, auto rhs) {
                return lhs->output_loc.value < rhs->output_loc.value;
            });
        auto draw_buffers = std::vector<Enum>(gsl::narrow<Enum>(max_location->output_loc.value + 1), GL_NONE);

        for (auto const& [texture, output_loc] : attachments)
        {
            auto attachment_name = GL_COLOR_ATTACHMENT0 + allocated_attachments++;
            draw_buffers[output_loc.value] = attachment_name;
            glFramebufferTexture(GL_FRAMEBUFFER, attachment_name, texture.id(), 0);
        }
        glDrawBuffers(static_cast<Size>(draw_buffers.size()), draw_buffers.data());
    }
}  // namespace glpp