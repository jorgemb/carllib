#include <cstddef>
#include <variant>

#include <catch2/catch_test_macros.hpp>

#include "carllib/ca/initialization.hpp"
#include "carllib/util/variant.hpp"

namespace init = carllib::initialization;
namespace util = carllib::util;

TEST_CASE("visitor pattern for initial condition", "[ca][initial_condition]") {
  // Standard start
  auto standard_start = init::initial_condition {init::standard {}};
  standard_start.visit(util::overload {
      [](init::standard&) -> void
      {
        // Expected, no action taken
      },
      [](init::randomized&) -> void { FAIL("Expecting a Standard value"); }});

  // Random start
  constexpr auto random_seed = std::size_t {42};
  auto random_start = init::initial_condition {init::randomized {random_seed}};
  random_start.visit(util::overload {[](init::standard&) -> void
                                     { FAIL("Standard not expected"); },
                                     [](init::randomized& random) -> void
                                     { CHECK(random.seed == random_seed); }});
}
