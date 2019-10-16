#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>
#include "glpp/id.hpp"
#include "glpp/primitive_types.hpp"

namespace glpp
{
    class Texture
    {
      public:
        struct Type
        {
            Enum internal_format;
            Enum format;
        };

        static constexpr Type rgba = {GL_RGBA8, GL_RGBA};
        static constexpr Type rgb = {GL_RGB8, GL_RGB};
        static constexpr Type r = {GL_R8, GL_RED};

        Texture() noexcept;

        Texture(
            int width,
            int height,
            std::uint8_t const* data = nullptr,
            Type type = rgba,
            bool tile = true,
            bool mipmap = true) noexcept;

        void load(
            int width,
            int height,
            std::uint8_t const* data = nullptr,
            Type type = rgba) noexcept;

        void enable_tiling() noexcept;

        void enable_linear_mipmapping() noexcept;

        void bind() const noexcept
        {
            glBindTexture(GL_TEXTURE_2D, id());
        }
        
		void unbind() const noexcept
        {
            glBindTexture(GL_TEXTURE_2D, nullid);
        }

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

        [[nodiscard]] auto width() const noexcept -> int { return width_; }

        [[nodiscard]] auto height() const noexcept -> int { return height_; }

      private:
        struct Deleter
        {
            void operator()(UInt32 size, Id* data) const noexcept;
        };

        UniqueIdArray<1, Deleter> id_;
        int width_;
        int height_;
    };

}  // namespace glpp