#include <string>

#include "carllib/carllib.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("carllib") == exported.name() ? 0 : 1;
}
