#pragma once

#include <array>
#include <cassert>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include <gsl/gsl_util>
#include <gsl/span>
#include "glpp/id.hpp"
#include "glpp/traits.hpp"

namespace glpp
{
    enum class BufferType : Enum
    {
        attrib_buffer = GL_ARRAY_BUFFER,
        index_buffer = GL_ELEMENT_ARRAY_BUFFER,
    };

    template <typename T, BufferType type>
    class BufferView
    {
      public:
        BufferView(Id id, std::ptrdiff_t offset, std::ptrdiff_t size) noexcept
          : id_{id}, offset_{offset}, size_{size}
        {
        }

        void bind() const noexcept
        {
            glBindBuffer(static_cast<Enum>(type), id_);
        }

        static void unbind() noexcept
        {
            glBindBuffer(static_cast<Enum>(type), nullid);
        }

        [[nodiscard]] auto offset() const noexcept -> std::ptrdiff_t { return offset_; }

        [[nodiscard]] auto size() const noexcept -> std::ptrdiff_t { return size_; }

      private:
        Id id_;
        std::ptrdiff_t offset_;
        std::ptrdiff_t size_;
    };

    template <typename T, BufferType type>
    class BufferBase
    {
      public:
        BufferBase() = default;

        [[nodiscard]] auto view() const noexcept -> BufferView<T, type>
        {
            return view(size_, 0);
        }

        [[nodiscard]] auto view(
            std::ptrdiff_t size,
            std::ptrdiff_t offset = 0)
            const noexcept -> BufferView<T, type>
        {
            assert(size_ >= offset + size);
            return BufferView<T, type>{id(), offset, size};
        }

        void bind() const noexcept
        {
            view().bind();
        }

        static void unbind() noexcept
        {
            BufferView<T, type>::unbind();
        }

        [[nodiscard]] auto size() const noexcept -> std::ptrdiff_t { return size_; }

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

      protected:
        void set_size(std::ptrdiff_t size) noexcept { size_ = size; }

      private:
        struct Deleter
        {
            void operator()(UInt32 size, Id* data) const noexcept
            {
                glDeleteBuffers(size, data);
            }
        };

        UniqueIdArray<1, Deleter> id_{glGenBuffers};
        std::ptrdiff_t size_ = 0;
    };

    template <typename T, BufferType type>
    class StaticBuffer : public BufferBase<T, type>
    {
      public:
        StaticBuffer() = default;

        void buffer_data(gsl::span<T const> data)
        {
            this->set_size(data.size());

            this->bind();
            glBufferData(
                static_cast<Enum>(type),
                data.size() * sizeof(T),
                data.data(),
                GL_STATIC_DRAW);
        }
    };

    template <typename T, BufferType type>
    class DynamicBuffer : public BufferBase<T, type>
    {
      public:
        DynamicBuffer() = default;

        void buffer_data(gsl::span<T const> data) noexcept
        {
            this->set_size(data.size());

            if (this->size() > capacity_)
            {
                reserve(std::max(
                    this->size(),
                    static_cast<std::ptrdiff_t>(capacity_ * growth_factor)));
            }

            buffer_subdata(data);
        }

        void buffer_subdata(gsl::span<T const> data, std::ptrdiff_t offset = 0) noexcept
        {
            assert(offset + data.size() <= this->size());
            this->bind();
            glBufferSubData(
                static_cast<Enum>(type),
                offset * sizeof(T),
                data.size() * sizeof(T),
                data.data());
        }

        void reserve(std::ptrdiff_t capacity) noexcept
        {
            capacity_ = std::max(capacity_, capacity);
            this->bind();
            glBufferData(
                static_cast<Enum>(type),
                capacity_ * sizeof(T),
                nullptr,
                GL_DYNAMIC_DRAW);
        }

        [[nodiscard]] auto capacity() const noexcept -> std::ptrdiff_t { return capacity_; }

      private:
        static constexpr auto growth_factor = 1.6;

        std::ptrdiff_t capacity_ = 0;
    };

    template <typename T>
    using AttribBufferView = BufferView<T, BufferType::attrib_buffer>;

    template <typename T>
    using IndexBufferView = BufferView<T, BufferType::index_buffer>;

    template <typename T>
    using StaticAttribBuffer = StaticBuffer<T, BufferType::attrib_buffer>;

    template <typename T>
    using StaticIndexBuffer = StaticBuffer<T, BufferType::index_buffer>;

    template <typename T>
    using DynamicAttribBuffer = DynamicBuffer<T, BufferType::attrib_buffer>;

    template <typename T>
    using DynamicIndexBuffer = DynamicBuffer<T, BufferType::index_buffer>;
}  // namespace glpp