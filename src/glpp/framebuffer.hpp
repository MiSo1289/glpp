#pragma once

#include <glad/glad.h>
#include <gsl/span>
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

        void set_frag_output_textures(gsl::span<TextureAttachment const> attachments) noexcept;

        void set_depth_texture(Texture& texture) noexcept
        {
            bind();
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.id(), 0);
        }

        void bind() const noexcept
        {
            glBindFramebuffer(GL_FRAMEBUFFER, id());
        }

        void unbind() const noexcept
        {
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
    };

}  // namespace glpp