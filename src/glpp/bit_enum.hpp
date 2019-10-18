#pragma once

#include <type_traits>

#define GLPP_MAKE_ENUM_BINARY_OPERATOR(T, Op)                              \
    [[nodiscard]] inline auto operator Op(T lhs, T rhs) noexcept->T        \
    {                                                                      \
        using Underlying = std::underlying_type_t<T>;                      \
        return static_cast<T>(                                             \
            static_cast<Underlying>(lhs) Op static_cast<Underlying>(rhs)); \
    }

#define GLPP_MAKE_ENUM_BINARY_MODIFYING_OPERATOR(T, Op)                       \
    [[nodiscard]] inline auto operator Op(T& lhs, T rhs) noexcept->T&         \
    {                                                                         \
        using Underlying = std::underlying_type_t<T>;                         \
        auto val = static_cast<Underlying>(lhs);                              \
        return (lhs = static_cast<T>((val Op static_cast<Underlying>(rhs)))); \
    }

#define GLPP_MAKE_BIT_ENUM(T)                       \
    GLPP_MAKE_ENUM_BINARY_OPERATOR(T, |)            \
    GLPP_MAKE_ENUM_BINARY_OPERATOR(T, &)            \
    GLPP_MAKE_ENUM_BINARY_MODIFYING_OPERATOR(T, |=) \
    GLPP_MAKE_ENUM_BINARY_MODIFYING_OPERATOR(T, &=)
