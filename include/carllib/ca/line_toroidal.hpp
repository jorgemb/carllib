//
// Created by jorge on 09/06/2026.
//

#ifndef CARLLIB_LINE_TOROIDAL_HPP
#define CARLLIB_LINE_TOROIDAL_HPP
#include <array>
#include <cstdint>

#include "carllib/util/math.hpp"

namespace carllib::ca
{

/**
 * Represents a 1D cellular automaton with toroidal wrapping
 */
template<class StoredValue, std::size_t Width, std::size_t Height>
class line_toroidal {
public:
  /**
   * Default constructor
   */
  line_toroidal() = default;

  /**
   * Returns the stored value at the given position. If the address is
   * out-of-bounds then it is wrapped around
   * @param column
   * @param row
   * @return
   */
  auto get_value_at(std::int32_t column, std::int32_t row) -> StoredValue {
    const auto col_index =
        static_cast<std::size_t>(util::wrap<std::int32_t>(column, 0, Width));
    const auto row_index =
        static_cast<std::size_t>(util::wrap<std::int32_t>(row, 0, Height));

    return m_data.at(row_index).at(col_index);
  }

  /**
   * Tries to calculate the next generation. Will return False if it is out of
   * space.
   * @return
   */
  auto next_generation() -> bool { return false; }

  /**
   * Returns the ID of the current generation (last to be simulated)
   * @return
   */
  auto current_generation() const -> std::size_t {
    return m_current_generation;
  }

  // Getters
  auto width() const -> std::size_t { return Width; }

  auto height() const -> std::size_t { return Height; }

private:
  // Stored values
  using row_type = std::array<StoredValue, Width>;
  std::array<row_type, Height> m_data;

  // State
  std::size_t m_current_generation = 0;
};

}  // namespace carllib::ca

#endif  // CARLLIB_LINE_TOROIDAL_HPP
