#pragma once

#include <cstddef>
#include <optional>

#include <glad/glad.h>
#include <gsl/span>
#include "glpp/gl.hpp"
#include "glpp/id.hpp"
#include "glpp/primitive_types.hpp"
#include "glpp/scoped_bind.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/texture.hpp"

namespace glpp
{
    struct TextureAttachment
    {
        Texture& texture;
        FragOutputLocation output_loc;
    };

    // todo: renderbuffers (at least for depth)
    class Framebuffer
    {
      public:
        Framebuffer() noexcept
          : id_{glGenFramebuffers} {}

        void set_viewport_size(ViewportSize const size)
        {
            size_ = size;
        }

        void set_frag_output_textures(gsl::span<TextureAttachment const> attachments) noexcept;

        void set_depth_texture(Texture& texture) noexcept
        {
            bind();
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.id(), 0);
        }

        void bind() const noexcept
        {
            prev_viewport_size_ = get_viewport_size();
            glpp::set_viewport_size(size_);
            glBindFramebuffer(GL_FRAMEBUFFER, id());
        }

        void unbind() const noexcept
        {
            glpp::set_viewport_size(
                std::exchange(prev_viewport_size_, std::nullopt)
                    .value());

            glBindFramebuffer(GL_FRAMEBUFFER, nullid);
        }

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

      private:
        struct Deleter
        {
            void operator()(UInt32 size, Id* data) const noexcept
            {
                glDeleteFramebuffers(size, data);
            }
        };

        UniqueIdArray<1, Deleter> id_;
        ViewportSize size_ = {};
        mutable std::optional<ViewportSize> prev_viewport_size_ = std::nullopt;
    };

}  // namespace glpp