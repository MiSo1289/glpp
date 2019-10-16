#pragma once

#include <stdexcept>

namespace glpp
{
    class Error : public std::runtime_error
    {
      public:
        using runtime_error::runtime_error;
    };

    class InitError : public Error
    {
      public:
        using Error::Error;
    };

	class ShaderCompilationError : public Error
    {
      public:
        using Error::Error;
    };
}  // namespace glpp