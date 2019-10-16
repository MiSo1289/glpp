#pragma once

#include "glpp/error.hpp"

namespace glpp::config
{
    class ConfigError : public Error
    {
      public:
        using Error::Error;
    };
}  // namespace glpp::config