#pragma once

#include <memory>
#include <type_traits>
#include <variant>

#include "glpp/primitive_types.hpp"
#include "glpp/traits.hpp"

namespace glpp
{
    template <typename ErasedPtr, typename VariantPtr>
    class BasicValuePtr
    {
      public:
        BasicValuePtr() noexcept = default;

        BasicValuePtr(std::nullptr_t) noexcept {}

        template <
            typename T,
            typename = std::enable_if_t<std::is_convertible_v<T*, VariantPtr>>>
        BasicValuePtr(T* const value) noexcept
          : value_{value}
        {
        }

        [[nodiscard]] auto get() const noexcept -> ErasedPtr
        {
            return std::visit(
                [](auto const value) -> ErasedPtr { return value; },
                value_);
        }

        [[nodiscard]] auto enumerator() const noexcept -> Enum
        {
            return std::visit(
                [](auto const value) -> Enum {
                    return primitive_type_enumerator_v<
                        std::decay_t<decltype(*value)>>;
                },
                value_);
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return get() != nullptr;
        }

      private:
        VariantPtr value_ = {};
    };

    using ValuePtr = BasicValuePtr<
        void*,
        std::variant<
            UInt8*,
            UInt16*,
            UInt32*,
            Int8*,
            Int16*,
            Int32*,
            Float32*,
            Float64*>>;

    using ConstValuePtr = BasicValuePtr<
        void const*,
        std::variant<
            UInt8 const*,
            UInt16 const*,
            UInt32 const*,
            Int8 const*,
            Int16 const*,
            Int32 const*,
            Float32 const*,
            Float64 const*>>;
}  // namespace glpp