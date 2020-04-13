#include "glpp/texture.hpp"

#include <array>

#include "glpp/scoped_bind.hpp"
#include "glpp/traits.hpp"

namespace glpp
{
    Texture::Texture(
        Data const data,
        Filter const filter,
        WrapBehaviour const wrap_behaviour,
        SwizzleMask const swizzle) noexcept
      : Texture{
          data,
          data.format,
          filter,
          wrap_behaviour,
          swizzle,
      }
    {
    }

    Texture::Texture(
        Data const data,
        InternalFormat const internal_format,
        Filter const filter,
        WrapBehaviour const wrap_behaviour,
        SwizzleMask const swizzle) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};

        do_load(data, internal_format, 0);
        do_set_filter(filter);
        do_set_wrap_behaviour(wrap_behaviour);
        do_set_swizzle(swizzle);

        if (std::holds_alternative<MipmapFilterType>(filter.min))
        {
            do_generate_mipmap();
        }
    }

    void Texture::load(
        Data const data,
        Int32 const level) noexcept
    {
        load(data, data.format, level);
    }

    void Texture::load(
        Data const data,
        InternalFormat const internal_format,
        Int32 const level) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};
        
        do_load(data, internal_format, level);
    }

    void Texture::update(
        Data const data,
        Int32 const x_offset,
        Int32 const y_offset,
        Int32 const level) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};

        glTexSubImage2D(
            GL_TEXTURE_2D,
            level,
            x_offset,
            y_offset,
            data.width,
            data.height,
            static_cast<Enum>(data.format),
            data.data.enumerator(),
            data.data.get());
    }

    void Texture::set_filter(Filter const filter) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};

        do_set_filter(filter);
    }

    void Texture::set_wrap_behaviour(WrapBehaviour const wrap_behaviour) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};

        do_set_wrap_behaviour(wrap_behaviour);
    }

    void Texture::set_swizzle(SwizzleMask const mask) noexcept
    {
        auto const binding = glpp::ScopedBind{*this};

        do_set_swizzle(mask);
    }

    void Texture::generate_mipmap() noexcept
    {
        auto const binding = glpp::ScopedBind{*this}; 
        do_generate_mipmap();
    }

    void Texture::Deleter::operator()(UInt32 const size, Id* const data) const noexcept
    {
        glDeleteTextures(size, data);
    }

    void Texture::do_load(
        Data const data,
        InternalFormat const internal_format,
        Int32 const level) noexcept
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            level,
            std::visit(
                [](auto const internal_format) {
                    return static_cast<Enum>(internal_format);
                },
                internal_format),
            data.width,
            data.height,
            0,
            static_cast<Enum>(data.format),
            data.data.enumerator(),
            data.data.get());

        if (level == 0)
        {
            width_ = data.width;
            height_ = data.height;
        }
    }

    void Texture::do_generate_mipmap() noexcept
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::do_set_filter(Filter const filter) noexcept
    {
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            std::visit(
                [](auto const filter_type) {
                    return static_cast<Enum>(filter_type);
                },
                filter.min));
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            static_cast<Enum>(filter.mag));
    }

    void Texture::do_set_wrap_behaviour(WrapBehaviour const wrap_behaviour) noexcept
    {
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            static_cast<Enum>(wrap_behaviour.s));
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            static_cast<Enum>(wrap_behaviour.t));
    }

    void Texture::do_set_swizzle(SwizzleMask const mask) noexcept
    {
        auto const mask_array = std::array<Int32, 4>{
            static_cast<Int32>(mask.r),
            static_cast<Int32>(mask.g),
            static_cast<Int32>(mask.b),
            static_cast<Int32>(mask.a),
        };
        glTexParameteriv(
            GL_TEXTURE_2D,
            GL_TEXTURE_SWIZZLE_RGBA,
            mask_array.data());
    }
}  // namespace glpp
