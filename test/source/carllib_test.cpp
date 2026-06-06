#include <string>

#include <catch2/catch_test_macros.hpp>

#include "carllib/carllib.hpp"

TEST_CASE("exported_class reports its name", "[carllib]")
{
  auto const exported = exported_class {};
  CHECK(exported.name() == std::string("carllib"));
}
