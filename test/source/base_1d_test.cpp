#include <cstddef>
#include <stdexcept>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "carllib/ca/base_1d.hpp"

namespace
{

class test_int_ca : public carllib::ca::base_1d<int> {
public:
  using base_1d::base_1d;

  // Trivial "increment every cell" rule used to exercise virtual dispatch
  // and the protected add_generation hook.
  auto calculate_next_generation() -> std::size_t override
  {
    auto next = get_generation_data(total_generations() - 1);
    for (auto& v : next) {
      ++v;
    }
    return add_generation(std::move(next));
  }

  // Public wrapper that lets tests reach the protected add_generation
  // and observe its width-mismatch error path.
  auto add_for_test(std::vector<int> g) -> std::size_t
  {
    return add_generation(std::move(g));
  }
};

class test_bool_ca : public carllib::ca::base_1d<bool> {
public:
  using base_1d::base_1d;

  auto calculate_next_generation() -> std::size_t override
  {
    auto copy = get_generation_data(total_generations() - 1);
    return add_generation(std::move(copy));
  }
};

}  // namespace

TEST_CASE("base_1d initializer-list constructor stores the initial generation",
          "[ca][base_1d]")
{
  test_int_ca ca {1, 2, 3, 4};

  CHECK(ca.width() == 4);
  CHECK(ca.total_generations() == 1);

  auto const& gen0 = ca.get_generation_data(0);
  REQUIRE(gen0.size() == 4);
  CHECK(gen0[0] == 1);
  CHECK(gen0[1] == 2);
  CHECK(gen0[2] == 3);
  CHECK(gen0[3] == 4);
}

TEST_CASE("base_1d<bool> width constructor seeds a single true cell in the middle",
          "[ca][base_1d]")
{
  test_bool_ca ca(std::size_t {7});

  CHECK(ca.width() == 7);
  CHECK(ca.total_generations() == 1);

  auto const& gen0 = ca.get_generation_data(0);
  REQUIRE(gen0.size() == 7);
  for (std::size_t i = 0; i < gen0.size(); ++i) {
    if (i == 3) {
      CHECK(gen0[i] == true);
    } else {
      CHECK(gen0[i] == false);
    }
  }
}

TEST_CASE("base_1d<bool> width constructor handles even widths", "[ca][base_1d]")
{
  test_bool_ca ca(std::size_t {4});

  auto const& gen0 = ca.get_generation_data(0);
  REQUIRE(gen0.size() == 4);
  // width / 2 == 2 for width 4, so the seeded cell is at index 2.
  CHECK(gen0[0] == false);
  CHECK(gen0[1] == false);
  CHECK(gen0[2] == true);
  CHECK(gen0[3] == false);
}

TEST_CASE("calculate_next_generation dispatches virtually and grows history",
          "[ca][base_1d]")
{
  test_int_ca ca {0, 0, 0};

  auto const new_count = ca.calculate_next_generation();
  CHECK(new_count == 2);
  CHECK(ca.total_generations() == 2);

  auto const& gen1 = ca.get_generation_data(1);
  REQUIRE(gen1.size() == 3);
  CHECK(gen1[0] == 1);
  CHECK(gen1[1] == 1);
  CHECK(gen1[2] == 1);

  ca.calculate_next_generation();
  auto const& gen2 = ca.get_generation_data(2);
  CHECK(gen2[0] == 2);
  CHECK(gen2[1] == 2);
  CHECK(gen2[2] == 2);
}

TEST_CASE("add_generation rejects generations whose width does not match",
          "[ca][base_1d]")
{
  test_int_ca ca {1, 2, 3};

  CHECK_THROWS_AS(ca.add_for_test(std::vector<int> {1, 2}), std::runtime_error);
  CHECK_THROWS_AS(ca.add_for_test(std::vector<int> {1, 2, 3, 4}),
                  std::runtime_error);

  // Previous generations should be untouched after a rejected add.
  CHECK(ca.total_generations() == 1);
}

TEST_CASE("add_generation accepts a matching-width generation", "[ca][base_1d]")
{
  test_int_ca ca {1, 2, 3};

  auto const new_count = ca.add_for_test(std::vector<int> {9, 8, 7});
  CHECK(new_count == 2);
  CHECK(ca.total_generations() == 2);

  auto const& gen1 = ca.get_generation_data(1);
  CHECK(gen1[0] == 9);
  CHECK(gen1[1] == 8);
  CHECK(gen1[2] == 7);
}