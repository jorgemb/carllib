//
// Created by jorge on 11/06/2026.
//

#include "carllib/ca/wolfram.hpp"

namespace carllib::ca
{
wolfram::wolfram(const wolfram_number rule, const std::size_t width, initial_condition init_condition)
    : base_1d(width, init_condition)
    , m_rule_number(rule) {}

auto wolfram::calculate_next_generation() -> std::size_t {
  // Reserve next generation space
  auto const previous_generation = get_generation_data(total_generations() - 1);
  auto next_generation = std::vector<bool>();
  next_generation.reserve(width());

  // Calculate triad value (left neighbor, own, right neighbor) as 111.
  // Use `unsigned` so the integer promotion done by the bitwise operators
  // does not turn the operands into signed `int` (hicpp-signed-bitwise).
  auto left_neighbor = static_cast<unsigned>(previous_generation.back());
  for (auto iter = previous_generation.begin();
       iter != previous_generation.end();
       ++iter)
  {
    auto const current = static_cast<unsigned>(*iter);
    auto const right_neighbor = (iter + 1) == previous_generation.end()
        ? static_cast<unsigned>(previous_generation.front())
        : static_cast<unsigned>(*(iter + 1));

    // Triad value is the bit encoded field of the two neighbors and the current
    // For example, if left==true, current==false and right==true the value
    // would be 101.
    const auto triad_value =
        left_neighbor << 2U | current << 1U | right_neighbor;

    // The rule encodes whether a given triad is true or false
    const auto rule = static_cast<unsigned>(m_rule_number.value);
    const auto new_value = ((rule >> triad_value) & 1U) > 0;
    next_generation.push_back(new_value);

    // Assign left neighbor
    left_neighbor = current;
  }

  return add_generation(std::move(next_generation));
}
}  // namespace carllib::ca
