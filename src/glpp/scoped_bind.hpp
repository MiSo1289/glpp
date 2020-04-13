#pragma once

#include <cstddef>
#include <new>
#include <utility>

#include <gsl/gsl_util>

namespace glpp
{
    // Binds object in constructor;
    // Restores the previous binding or unbinds if
    // no previous binding exists in destructor
    template <typename Bindable>
    class ScopedBind
    {
      public:
        explicit ScopedBind(Bindable& bindable) noexcept
          : binded_{bindable}
          , prev_binding_{std::exchange(active_binding, this)}
        {
            bindable.bind();

            if (prev_binding_)
            {
                prev_binding_->next_binding_ = this;
            }
        }

        ScopedBind(ScopedBind&&) = delete;
        ScopedBind(ScopedBind const&) = delete;

        ~ScopedBind() noexcept
        {
            if (next_binding_)
            {
                next_binding_->prev_binding_ = prev_binding_;
            }
            else
            {
                active_binding = prev_binding_;

                binded_.unbind();
            }

            if (prev_binding_)
            {
                prev_binding_->next_binding_ = next_binding_;
                prev_binding_->binded_.bind();
            }
            else
            {
                std::remove_const_t<Bindable>::unbind();
            }
        }

        auto operator=(ScopedBind&&) = delete;
        auto operator=(ScopedBind const&) = delete;

      private:
        static inline thread_local ScopedBind* active_binding = nullptr;

        Bindable& binded_;
        ScopedBind* prev_binding_;
        // Note: a doubly linked list is required
        // for this class to be completely memory safe:
        // Destructors are called out of order when a function
        // creates two binding and returns the second one using copy ellision,
        // or when dynamically allocating a binding
        // (we can delete operator new, but one could still create
        // a binding as a subobject of a dynamically allocated object).
        ScopedBind* next_binding_ = nullptr;
    };

    // A simpler alternative to scoped bind:
    // Does not restore the previous binding in destructor;
    // always unbinds instead.
    template <typename Bindable>
    class UniqueBind
    {
      public:
        explicit UniqueBind(Bindable& bindable) noexcept
          : binded_{bindable}
        {
            bindable.bind();
        }

        UniqueBind(UniqueBind&&) = delete;
        UniqueBind(UniqueBind const&) = delete;

        ~UniqueBind() noexcept
        {
            std::remove_const_t<Bindable>::unbind();
        }

        auto operator=(UniqueBind&&) = delete;
        auto operator=(UniqueBind const&) = delete;

      private:
        Bindable& binded_;
    };
}  // namespace glpp