#include "glpp/texture.hpp"

namespace glpp
{
    Texture::Texture() noexcept
      : id_{glGenTextures} {}

    Texture::Texture(
        int width,
        int height,
        std::uint8_t const* data,
        Texture::Type type,
        bool tile,
        bool mipmap) noexcept
      : Texture{}
    {
        load(width, height, data, type);
        if (tile) enableTiling();
        if (mipmap) enableLinearMipmapping();
    }

    void Texture::load(
        int width,
        int height,
        std::uint8_t const* data,
        Texture::Type type) noexcept
    {
        bind();
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            type.internalFormat,
            width,
            height,
            0,
            type.format,
            GL_UNSIGNED_BYTE,
            data);

        width_ = width;
        height_ = height;
    }

    void Texture::enable_tiling() noexcept
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::enable_linear_mipmapping() noexcept
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::Deleter::operator()(UInt32 size, Id* data) const noexcept
    {
        glDeleteTextures(size, data);
    }

}  // namespace glpp