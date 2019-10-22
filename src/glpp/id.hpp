#pragma once

#include <algorithm>
#include <array>
#include <type_traits>

#include <glad/glad.h>
#include "glpp/primitive_types.hpp"

namespace glpp
{
    using Id = UInt32;

    class NullId
    {
      public:
        operator Id() const { return 0u; }
    };

    inline constexpr auto nullid = NullId{};

    template <typename Deleter>
    class UniqueId
    {
      public:
        UniqueId() = default;

        explicit UniqueId(Id id, Deleter deleter = {}) noexcept
          : id_{id}, deleter_{std::move(deleter)} {}

        UniqueId(UniqueId const&) = delete;

        UniqueId(UniqueId&& other) noexcept { swap(*this, other); }

        ~UniqueId() noexcept { deleter_(id_); }

        auto operator=(UniqueId other) noexcept -> UniqueId&
        {
            swap(*this, other);
            return *this;
        }

        friend void swap(UniqueId& left, UniqueId& right) noexcept
        {
            using std::swap;
            swap(left.deleter_, right.deleter_);
            swap(left.id_, right.id_);
        }

        [[nodiscard]] auto get() const -> Id { return id_; }

      private:
        Id id_ = nullid;
        Deleter deleter_;
    };

    template <Id size, typename Deleter>
    class UniqueIdArray
    {
      public:
        UniqueIdArray() = default;

        template <typename Creator,
                  typename = std::enable_if_t<std::is_default_constructible_v<Deleter>>>
        explicit UniqueIdArray(Creator&& creator) noexcept
          : UniqueIdArray{std::forward<Creator>(creator), Deleter{}}
        {
        }

        template <typename Creator>
        UniqueIdArray(Creator&& creator, Deleter deleter)
          : deleter_{std::move(deleter)}
        {
            creator(size, ids_.data());
        }

        UniqueIdArray(UniqueIdArray const&) = delete;

        UniqueIdArray(UniqueIdArray&& other) noexcept { swap(other); }

        ~UniqueIdArray() noexcept { deleter_(size, ids_.data()); }

        auto operator=(UniqueIdArray other) noexcept -> UniqueIdArray&
        {
            swap(*this, other);
            return *this;
        }

        friend void swap(UniqueIdArray& left, UniqueIdArray& right) noexcept
        {
            using std::swap;

            swap(left.deleter_, right.deleter_);
            swap(left.ids_, right.ids_);
        }

        [[nodiscard]] auto get(std::size_t i = 0) const -> Id { return ids_[i]; }

      private:
        std::array<Id, size> ids_ = {};
        Deleter deleter_;
    };

}  // namespace glpp
