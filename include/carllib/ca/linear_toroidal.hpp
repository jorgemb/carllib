//
// Created by jorge on 09/06/2026.
//

#ifndef CARLLIB_LINE_TOROIDAL_HPP
#define CARLLIB_LINE_TOROIDAL_HPP
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>

#include "carllib/util/math.hpp"

namespace carllib::ca
{

/**
 * Represents a 1D cellular automaton with toroidal wrapping
 */
template<class StoredValue>
class linear_toroidal {
public:
  using generation = std::vector<StoredValue>;

  /**
   * Default starting point, single true cell in the middel
   * @param width
   */

  explicit linear_toroidal(const std::size_t width) requires(std::is_same_v<StoredValue, bool>) : m_width(width) {
    // Calculate mid-point
    const auto middle = width / 2;
    auto starting_generation = std::vector(width, false);
    starting_generation.at(middle) = true;
    m_data.push_back(std::move(starting_generation));
  }

  /**
   * Constructor with initializer list
   */
  linear_toroidal(std::initializer_list<StoredValue> initial_generation)
      : m_width(initial_generation.size()) {
    m_data.emplace_back(std::move(initial_generation));
  }

  /**
   * Iterator constructor
   * @tparam Iterator
   * @param begin
   * @param end
   */
  template<std::input_iterator Iterator>
  linear_toroidal(Iterator begin, Iterator end)
      : m_width(std::distance(begin, end)) {
    m_data.emplace_back({begin, end});
  }

  // Copy and move operations
  linear_toroidal(const linear_toroidal& other) = default;
  linear_toroidal(linear_toroidal&& other) noexcept = default;
  auto operator=(const linear_toroidal& other) -> linear_toroidal& = default;
  auto operator=(linear_toroidal&& other) noexcept
      -> linear_toroidal& = default;

  // Destructor
  virtual ~linear_toroidal() = default;

  /**
   * Returns the full data of the generation
   * @param number
   * @return
   */
  auto get_generation_data(std::size_t number) const -> const generation& {
    return m_data.at(number);
  }

  /**
   * Tries to calculate the next generation. Returns the new generation number.
   * space.
   * @return
   */
  virtual auto calculate_next_generation() -> std::size_t {
    // Copy the previous generation
    auto const &last = m_data.back();
    m_data.emplace_back(last.begin(), last.end());

    return m_data.size();
  }

  /**
   * Returns the total amount of calculated generations
   * @return
   */
  auto total_generations() const -> std::size_t { return m_data.size(); }

  // Getters
  auto width() const -> std::size_t { return m_width; }

private:
  // Stored values
  std::vector<generation> m_data;

  // State
  std::size_t m_width;
};

}  // namespace carllib::ca

#endif  // CARLLIB_LINE_TOROIDAL_HPP
