#pragma once

#include <utility>

#include <glad/glad.h>
#include "glpp/buffer.hpp"
#include "glpp/id.hpp"
#include "glpp/scoped_bind.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/traits.hpp"

namespace glpp
{
    class VertexArray
    {
      public:
        VertexArray() noexcept
          : id_{glGenVertexArrays} {}

        void bind() const noexcept { glBindVertexArray(id()); }

        void unbind() const noexcept { glBindVertexArray(nullid); }

        template <typename T>
        void bind_attribute_buffer(
            AttribBufferView<T> buff,
            AttributeLocation attribute_loc,
            GLuint num_components) noexcept
        {
            auto vao_bind = ScopedBind{*this};
            glEnableVertexAttribArray(attribute_loc.value);
            auto buff_bind = ScopedBind{buff};
            glVertexAttribPointer(
                attribute_loc.value,
                num_components,
                primitive_type_enumerator_v<T>,
                false,
                0,
                reinterpret_cast<void*>(buff.offset() * sizeof(T)));
        }

        void unbind_attribute_buffer(AttributeLocation attribute_loc) noexcept;

        [[nodiscard]] auto id() const noexcept -> Id { return id_.get(); }

      private:
        struct Deleter
        {
            void operator()(GLuint size, GLuint* data) const noexcept
            {
                glDeleteVertexArrays(size, data);
            }
        };

        UniqueIdArray<1, Deleter> id_;
    };

}  // namespace glpp