#include <cstdint>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "carllib/ca/wolfram.hpp"

using carllib::ca::wolfram;
using carllib::ca::wolfram_number;

namespace
{

auto gen_as_vector(wolfram const& ca, std::size_t n) -> std::vector<bool>
{
  auto const& gen = ca.get_generation_data(n);
  return {gen.begin(), gen.end()};
}

}  // namespace

TEST_CASE("wolfram seeds a single true cell in the middle", "[ca][wolfram]")
{
  wolfram ca {wolfram_number {0}, 7};

  CHECK(ca.width() == 7);
  CHECK(ca.total_generations() == 1);

  auto const gen0 = gen_as_vector(ca, 0);
  CHECK(gen0
        == std::vector<bool> {false, false, false, true, false, false, false});
}

TEST_CASE("wolfram rule 0 sends every cell to false", "[ca][wolfram]")
{
  wolfram ca {wolfram_number {0}, 7};

  ca.calculate_next_generation();

  CHECK(gen_as_vector(ca, 1)
        == std::vector<bool> {false, false, false, false, false, false, false});
}

TEST_CASE("wolfram rule 255 sends every cell to true", "[ca][wolfram]")
{
  wolfram ca {wolfram_number {255}, 5};

  ca.calculate_next_generation();
  ca.calculate_next_generation();

  CHECK(gen_as_vector(ca, 1) == std::vector<bool> {true, true, true, true, true});
  CHECK(gen_as_vector(ca, 2) == std::vector<bool> {true, true, true, true, true});
}

TEST_CASE("wolfram rule 90 produces the expected XOR step", "[ca][wolfram]")
{
  // Rule 90: next cell = left XOR right. Single seed in a width-7 ring should
  // expand symmetrically.
  wolfram ca {wolfram_number {90}, 7};

  ca.calculate_next_generation();

  CHECK(gen_as_vector(ca, 1)
        == std::vector<bool> {false, false, true, false, true, false, false});
}

TEST_CASE("wolfram rule 30 produces the expected first generation",
          "[ca][wolfram]")
{
  wolfram ca {wolfram_number {30}, 7};

  ca.calculate_next_generation();

  CHECK(gen_as_vector(ca, 1)
        == std::vector<bool> {false, false, true, true, true, false, false});
}

TEST_CASE("wolfram rule 110 matches the canonical first two generations",
          "[ca][wolfram]")
{
  wolfram ca {wolfram_number {110}, 7};

  ca.calculate_next_generation();
  ca.calculate_next_generation();

  CHECK(ca.total_generations() == 3);
  CHECK(gen_as_vector(ca, 1)
        == std::vector<bool> {false, false, true, true, false, false, false});
  CHECK(gen_as_vector(ca, 2)
        == std::vector<bool> {false, true, true, true, false, false, false});
}

TEST_CASE("wolfram wraps neighbors toroidally at both edges", "[ca][wolfram]")
{
  // Rule 90, width 4. Initial: [F,F,T,F] (middle seeded at index 2).
  // Cell 3's right neighbor wraps to cell 0; the resulting `true` at index 3
  // can only appear if the wrap is in effect.
  wolfram ca {wolfram_number {90}, 4};

  ca.calculate_next_generation();

  CHECK(gen_as_vector(ca, 1)
        == std::vector<bool> {false, true, false, true});
}

TEST_CASE("wolfram returns the new generation count from "
          "calculate_next_generation",
          "[ca][wolfram]")
{
  wolfram ca {wolfram_number {30}, 5};

  CHECK(ca.calculate_next_generation() == 2);
  CHECK(ca.calculate_next_generation() == 3);
  CHECK(ca.total_generations() == 3);
}