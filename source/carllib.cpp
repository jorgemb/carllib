#include <string>

#include "carllib/carllib.hpp"

exported_class::exported_class()
    : m_name {"carllib"}
{
}

auto exported_class::name() const -> char const*
{
  return m_name.c_str();
}
