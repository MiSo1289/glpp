#pragma once

#include <optional>
#include <variant>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glpp/primitive_types.hpp"

namespace glpp
{
    enum class SimpleBlendEquation : Enum
    {
        min = GL_MIN,
        max = GL_MAX,
    };

    enum class ParameterizedBlendEquationType : Enum
    {
        add = GL_FUNC_ADD,
        subtract = GL_FUNC_SUBTRACT,
        reverse_subtract = GL_FUNC_REVERSE_SUBTRACT,
    };

    enum class BlendParameter : Enum
    {
        zero = GL_ZERO,
        one = GL_ONE,
        src_color = GL_SRC_COLOR,
        one_minus_src_color = GL_ONE_MINUS_SRC_COLOR,
        dst_color = GL_DST_COLOR,
        one_minus_dst_color = GL_ONE_MINUS_DST_COLOR,
        src_alpha = GL_SRC_ALPHA,
        one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA,
        dst_alpha = GL_DST_ALPHA,
        one_minus_dst_alpha = GL_ONE_MINUS_DST_ALPHA,
        src_alpha_saturate = GL_SRC_ALPHA_SATURATE,
        constant_color = GL_CONSTANT_COLOR,
        one_minus_constant_color = GL_ONE_MINUS_CONSTANT_COLOR,
        constant_alpha = GL_CONSTANT_ALPHA,
        one_minus_constant_alpha = GL_ONE_MINUS_CONSTANT_ALPHA,
        src1_color = GL_SRC1_COLOR,
        one_minus_src1_color = GL_ONE_MINUS_SRC1_COLOR,
        src1_alpha = GL_SRC1_ALPHA,
        one_minus_src1_alpha = GL_ONE_MINUS_SRC1_ALPHA,
    };

    struct ParameterizedBlendEquation
    {
        ParameterizedBlendEquationType type
            = ParameterizedBlendEquationType::add;
        BlendParameter src_param = BlendParameter::one;
        BlendParameter dst_param = BlendParameter::zero;
    };

    using BlendEquation = std::variant<
        SimpleBlendEquation,
        ParameterizedBlendEquation>;

    class BlendMode
    {
      public:
        explicit BlendMode(BlendEquation const equation)
          : BlendMode{equation, equation} {}

        BlendMode(
            BlendEquation const equation,
            glm::vec4 const constant_color)
          : BlendMode{equation, equation, constant_color} {}

        BlendMode(
            BlendEquation const color_equation,
            BlendEquation const alpha_equation)
          : color_equation_{get_equation_enumerators(color_equation)}
          , alpha_equation_{get_equation_enumerators(alpha_equation)} {}

        BlendMode(
            BlendEquation const color_equation,
            BlendEquation const alpha_equation,
            glm::vec4 const constant_color)
          : color_equation_{get_equation_enumerators(color_equation)}
          , alpha_equation_{get_equation_enumerators(alpha_equation)}
          , constant_color_{constant_color} {}

        void bind() const noexcept;

        static void unbind() noexcept;

      private:
        struct EquationEnumerators
        {
            Enum equation;
            Enum src_param;
            Enum dst_param;
        };

        EquationEnumerators color_equation_;
        EquationEnumerators alpha_equation_;
        std::optional<glm::vec4> constant_color_ = std::nullopt;

        [[nodiscard]] static auto get_equation_enumerators(
            BlendEquation equation) -> EquationEnumerators;
    };
}  // namespace glpp