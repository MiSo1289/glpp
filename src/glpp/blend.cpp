#include "glpp/blend.hpp"

namespace glpp
{
    void BlendMode::bind() const noexcept
    {
        glEnable(GL_BLEND);

        glBlendEquationSeparate(
            color_equation_.equation,
            alpha_equation_.equation);

        glBlendFuncSeparate(
            color_equation_.src_param,
            color_equation_.dst_param,
            alpha_equation_.src_param,
            alpha_equation_.dst_param);

        if (constant_color_.has_value())
        {
            glBlendColor(
                constant_color_->r,
                constant_color_->g,
                constant_color_->b,
                constant_color_->a);
        }
    }

    void BlendMode::unbind() noexcept
    {
        glBlendEquationSeparate(
            GL_FUNC_ADD,
            GL_FUNC_ADD);

        glBlendFuncSeparate(
            GL_ONE,
            GL_ZERO,
            GL_ONE,
            GL_ZERO);

        glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        glDisable(GL_BLEND);
    }

    auto BlendMode::get_equation_enumerators(
        BlendEquation const equation) -> EquationEnumerators
    {
        if (auto const* const simple_equation
            = std::get_if<SimpleBlendEquation>(&equation))
        {
            return {
                static_cast<Enum>(*simple_equation),
                GL_ONE,
                GL_ZERO,
            };
        }

        auto const& parameterized_equation
            = std::get<ParameterizedBlendEquation>(equation);

        return {
            static_cast<Enum>(parameterized_equation.type),
            static_cast<Enum>(parameterized_equation.src_param),
            static_cast<Enum>(parameterized_equation.dst_param),
        };
    }
}  // namespace glpp