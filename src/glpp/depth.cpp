#include "glpp/depth.hpp"

namespace glpp
{
    void clear_depth(Float32 const depth) noexcept
    {
        glClearDepthf(depth);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void DepthMode::bind() const noexcept
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(static_cast<Enum>(predicate_));
        glDepthMask(access_ == DepthBufferAccess::read_write);
    }

    void DepthMode::unbind() const noexcept
    {
        glDepthMask(true);
        glDepthFunc(static_cast<Enum>(DepthPredicate::less));
        glDisable(GL_DEPTH_TEST);
	}
}  // namespace glpp
