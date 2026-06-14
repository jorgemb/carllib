//
// Created by jorge on 09/06/2026.
//

#ifndef CARLLIB_LINE_TOROIDAL_HPP
#define CARLLIB_LINE_TOROIDAL_HPP
#include <algorithm>
#include <random>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "carllib/util/math.hpp"

namespace carllib::ca
{

/**
 * Represents the initial condition to apply
 */
enum initial_condition
{
  standard,
  random
};

/**
 * Represents a 1D cellular automaton with toroidal wrapping
 */
template<class StoredValue>
class base_1d {
public:
  using generation = std::vector<StoredValue>;

  /**
   * Default starting point, single true cell in the middel
   * @param width
   * @param init_condition
   */
  explicit base_1d(
      const std::size_t width,
      initial_condition init_condition = initial_condition::standard)
    requires(std::is_same_v<StoredValue, bool>)
      : m_width(width) {
    switch (init_condition) {
      case initial_condition::standard: {
        // Calculate mid-point
        const auto middle = width / 2;
        auto starting_generation = std::vector(width, false);
        starting_generation.at(middle) = true;
        m_data.push_back(std::move(starting_generation));
        break;
      }
      case initial_condition::random: {
        auto random_engine =
            std::default_random_engine {std::random_device {}()};
        auto random_generator = std::bernoulli_distribution {0.5};
        auto starting_generation = std::vector(width, false);
        std::ranges::generate(starting_generation,
                              [&random_engine, &random_generator]() -> bool
                              { return random_generator(random_engine); });
        m_data.push_back(starting_generation);
        break;
      }
      default:
        throw std::runtime_error("Non recognized initial condition to base_1d");
    }
  }

  /**
   * Constructor with initializer list
   */
  base_1d(std::initializer_list<StoredValue> initial_generation)
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
  base_1d(Iterator begin, Iterator end)
      : m_width(std::distance(begin, end)) {
    m_data.emplace_back({begin, end});
  }

  // Copy and move operations
  base_1d(const base_1d& other) = default;
  base_1d(base_1d&& other) noexcept = default;
  auto operator=(const base_1d& other) -> base_1d& = default;
  auto operator=(base_1d&& other) noexcept -> base_1d& = default;

  // Destructor
  virtual ~base_1d() = default;

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
  virtual auto calculate_next_generation() -> std::size_t = 0;

  /**
   * Returns the total amount of calculated generations
   * @return
   */
  auto total_generations() const -> std::size_t { return m_data.size(); }

  // Getters
  auto width() const -> std::size_t { return m_width; }

protected:
  /**
   * Adds a new generation to the automaton. The new generation must have the
   * same width as the rest.
   * @param new_generation
   * @return
   */
  auto add_generation(std::vector<StoredValue>&& new_generation)
      -> std::size_t {
    if (new_generation.size() != m_width) {
      throw std::runtime_error(
          "Attempting to add a generation that is of wrong width");
    }

    m_data.push_back(std::move(new_generation));
    return m_data.size();
  }

private:
  // Stored values
  std::vector<generation> m_data;

  // State
  std::size_t m_width;
};

}  // namespace carllib::ca

#endif  // CARLLIB_LINE_TOROIDAL_HPP
