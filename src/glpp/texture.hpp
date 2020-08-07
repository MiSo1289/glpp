#pragma once

#include <cstdint>
#include <string>
#include <variant>

#include <glad/glad.h>
#include "glpp/id.hpp"
#include "glpp/primitive_types.hpp"
#include "glpp/uniform.hpp"
#include "glpp/value_ptr.hpp"

namespace glpp
{
    class Texture
    {
      public:
        enum class BasicFormat : Enum
        {
            rgba = GL_RGBA,
            bgra = GL_BGRA,
            rgb = GL_RGB,
            bgr = GL_BGR,
            rg = GL_RG,
            r = GL_RED,
            depth_component = GL_DEPTH_COMPONENT,
            depth_stencil = GL_DEPTH_STENCIL,
        };

        enum class SizedInternalFormat : Enum
        {
            r8 = GL_R8,
            r8_snorm = GL_R8_SNORM,
            r16 = GL_R16,
            r16_snorm = GL_R16_SNORM,
            rg8 = GL_RG8,
            rg8_snorm = GL_RG8_SNORM,
            rg16 = GL_RG16,
            rg16_snorm = GL_RG16_SNORM,
            r3_g3_b2 = GL_R3_G3_B2,
            rgb4 = GL_RGB4,
            rgb5 = GL_RGB5,
            rgb8 = GL_RGB8,
            rgb8_snorm = GL_RGB8_SNORM,
            rgb10 = GL_RGB10,
            rgb12 = GL_RGB12,
            rgb16_snorm = GL_RGB16_SNORM,
            rgba2 = GL_RGBA2,
            rgba4 = GL_RGBA4,
            rgb5_a1 = GL_RGB5_A1,
            rgba8 = GL_RGBA8,
            rgba8_snorm = GL_RGBA8_SNORM,
            rgb10_a2 = GL_RGB10_A2,
            rgb10_a2ui = GL_RGB10_A2UI,
            rgba12 = GL_RGBA12,
            rgba16 = GL_RGBA16,
            srgb8 = GL_SRGB8,
            srgb8_alpha8 = GL_SRGB8_ALPHA8,
            r16f = GL_R16F,
            rg16f = GL_RG16F,
            rgb16f = GL_RGB16F,
            rgba16f = GL_RGBA16F,
            r32f = GL_R32F,
            rg32f = GL_RG32F,
            rgb32f = GL_RGB32F,
            rgba32f = GL_RGBA32F,
            r11f_g11f_b10f = GL_R11F_G11F_B10F,
            rgb9_e5 = GL_RGB9_E5,
            r8i = GL_R8I,
            r8ui = GL_R8UI,
            r16i = GL_R16I,
            r16ui = GL_R16UI,
            r32i = GL_R32I,
            r32ui = GL_R32UI,
            rg8i = GL_RG8I,
            rg8ui = GL_RG8UI,
            rg16i = GL_RG16I,
            rg16ui = GL_RG16UI,
            rg32i = GL_RG32I,
            rg32ui = GL_RG32UI,
            rgb8i = GL_RGB8I,
            rgb8ui = GL_RGB8UI,
            rgb16i = GL_RGB16I,
            rgb16ui = GL_RGB16UI,
            rgb32i = GL_RGB32I,
            rgb32ui = GL_RGB32UI,
            rgba8i = GL_RGBA8I,
            rgba8ui = GL_RGBA8UI,
            rgba16i = GL_RGBA16I,
            rgba16ui = GL_RGBA16UI,
            rgba32i = GL_RGBA32I,
            rgba32ui = GL_RGBA32UI,
        };

        enum class CompressedInternalFormat : Enum
        {
            compressed_red = GL_COMPRESSED_RED,
            compressed_rg = GL_COMPRESSED_RG,
            compressed_rgb = GL_COMPRESSED_RGB,
            compressed_rgba = GL_COMPRESSED_RGBA,
            compressed_srgb = GL_COMPRESSED_SRGB,
            compressed_srgb_alpha = GL_COMPRESSED_SRGB_ALPHA,
            compressed_red_rgtc1 = GL_COMPRESSED_RED_RGTC1,
            compressed_signed_red_rgtc1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
            compressed_rg_rgtc2 = GL_COMPRESSED_RG_RGTC2,
            compressed_signed_rg_rgtc2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
            compressed_rgba_bptc_unorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
            compressed_srgb_alpha_bptc_unorm = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
            compressed_rgb_bptc_signed_float = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
            compressed_rgb_bptc_unsigned_float = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
        };

        using InternalFormat = std::variant<
            BasicFormat,
            SizedInternalFormat,
            CompressedInternalFormat>;

        struct Data
        {
            Size width;
            Size height;
            BasicFormat format = BasicFormat::rgba;
            ConstValuePtr data = nullptr;
        };

        enum class BasicFilterType : Enum
        {
            nearest = GL_NEAREST,
            linear = GL_LINEAR,
        };

        enum class MipmapFilterType : Enum
        {
            nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
            nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
            linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
            linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,
        };

        struct Filter
        {
            std::variant<BasicFilterType, MipmapFilterType>
                min = MipmapFilterType::nearest_mipmap_linear;
            BasicFilterType mag = BasicFilterType::linear;
        };

        enum class WrapBehaviourType : Enum
        {
            clamp_to_edge = GL_CLAMP_TO_EDGE,
            mirrored_repeat = GL_MIRRORED_REPEAT,
            repeat = GL_REPEAT,
        };

        struct WrapBehaviour
        {
            WrapBehaviourType s = WrapBehaviourType::repeat;
            WrapBehaviourType t = WrapBehaviourType::repeat;
        };

        enum class SwizzleChannel : Enum
        {
            red = GL_RED,
            green = GL_GREEN,
            blue = GL_BLUE,
            alpha = GL_ALPHA,
            zero = GL_ZERO,
            one = GL_ONE,
        };

        struct SwizzleMask
        {
            SwizzleChannel r = SwizzleChannel::red;
            SwizzleChannel g = SwizzleChannel::green;
            SwizzleChannel b = SwizzleChannel::blue;
            SwizzleChannel a = SwizzleChannel::alpha;
        };

        Texture() noexcept = default;

        // Sets internal format to data.format.
        explicit Texture(Data const data) noexcept
          : Texture{
              data,
              Filter{},
              WrapBehaviour{},
              SwizzleMask{},
          }
        {
        }

        Texture(
            Data const data,
            InternalFormat const internal_format) noexcept
          : Texture{
              data,
              internal_format,
              Filter{},
              WrapBehaviour{},
              SwizzleMask{},
          }
        {
        }

        // Sets internal format to data.format.
        // If filter.min is a mipmap filter, also calls generate_mipmap().
        Texture(
            Data data,
            Filter filter,
            WrapBehaviour wrap_behaviour,
            SwizzleMask swizzle) noexcept;

        // If filter.min is a mipmap filter, also calls generate_mipmap().
        Texture(
            Data data,
            InternalFormat internal_format,
            Filter filter,
            WrapBehaviour wrap_behaviour,
            SwizzleMask swizzle) noexcept;

        // Sets internal format to data.format.
        void load(
            Data data,
            Int32 level = 0) noexcept;

        void load(
            Data data,
            InternalFormat internal_format,
            Int32 level = 0) noexcept;

        // Load must be called first to allocate a big enough texture.
        void update(
            Data data,
            Int32 x_offset = 0,
            Int32 y_offset = 0,
            Int32 level = 0) noexcept;

        void set_filter(Filter filter) noexcept;

        void set_wrap_behaviour(WrapBehaviour wrap_behaviour) noexcept;

        void set_swizzle(SwizzleMask mask) noexcept;

        // Has to be called manually after calling set_filter(),
        // or updating the texture (when using a mipmap min filter).
        void generate_mipmap() noexcept;

        void bind(SamplerUnit const sampler_unit = {}) const noexcept
        {
            glActiveTexture(GL_TEXTURE0 + sampler_unit.index);
            glBindTexture(GL_TEXTURE_2D, id());
        }

        static void unbind(SamplerUnit const sampler_unit = {}) noexcept
        {
            glActiveTexture(GL_TEXTURE0 + sampler_unit.index);
            glBindTexture(GL_TEXTURE_2D, nullid);
        }

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

        [[nodiscard]] auto width() const noexcept -> Size { return width_; }

        [[nodiscard]] auto height() const noexcept -> Size { return height_; }

      private:
        struct Deleter
        {
            void operator()(UInt32 size, Id* data) const noexcept;
        };

        UniqueIdArray<1, Deleter> id_{glGenTextures};
        Size width_ = {};
        Size height_ = {};

        void do_load(
            Data data,
            InternalFormat internal_format,
            Int32 level) noexcept;

        static void do_generate_mipmap() noexcept;

        static void do_set_filter(Filter filter) noexcept;

        static void do_set_wrap_behaviour(WrapBehaviour wrap_behaviour) noexcept;

        static void do_set_swizzle(SwizzleMask mask) noexcept;
    };

}  // namespace glpp