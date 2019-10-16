#pragma once

#include <gsl/gsl_util>

namespace glpp
{
    // Binds object in constructor; unbinds in destructor.
    // Note that for types provided by this library,
    // the unbind operation does not restore the previously
    // active binding.
    template <typename Bindable>
    class ScopedBind
    {
      public:
        explicit ScopedBind(Bindable& bindable) noexcept
          : binded_{bindable}
        {
            bindable.bind();
        }

        ScopedBind(ScopedBind&&) = delete;
        ScopedBind(ScopedBind const&) = delete;

        ~ScopedBind() noexcept
        {
            binded_.unbind();
        }

        ScopedBind& operator=(ScopedBind&&) = delete;
        ScopedBind& operator=(ScopedBind const&) = delete;

      private:
        Bindable& binded_;
    };
}  // namespace glpp