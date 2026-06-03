//
// Created by jorge on 03/06/2026.
//

#ifndef CARLLIB_CELL_GRID_HPP
#define CARLLIB_CELL_GRID_HPP

#include <cstdint>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace carllib
{

class cell_grid: public sf::Drawable, public sf::Transformable{
public:
  /**
   * Create a new cell_grid of the given size
   * @param initial_size Initial size in cells
   * @param cell_size Size of each cell (assuming square)
   */
  cell_grid(sf::Vector2u initial_size, std::uint32_t cell_size);

  // Constructors and operations
  cell_grid(const cell_grid& other) = default;
  cell_grid(cell_grid&& other) noexcept = default;
  auto operator=(const cell_grid& other) -> cell_grid& = default;
  auto operator=(cell_grid&& other) noexcept -> cell_grid& = default;
  ~cell_grid() override = default;

  /**
   * Resizes the cell_grid to the new size. This will drop data from previous
   * iterations.
   * @param new_size New size in cells
   * @param cell_size
   */
  void resize(sf::Vector2u new_size, std::uint32_t cell_size);

  /**
   * Sets the color in the given position. Returns False if the position
   * is invalid.
   * @param position
   * @param color
   * @return
   */
  auto set_color(const sf::Vector2u& position, sf::Color color) -> bool;

  /**
   * Gets the color at the given position. Returns std::nullopt in case
   * of an invalid position.
   * @param position
   * @return
   */
  auto get_color(const sf::Vector2u& position) -> std::optional<sf::Color>;

protected:
  /**
   * Draws on the render target
   * @param target
   * @param states
   */
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  // Size
  sf::Vector2u m_size;
  std::uint32_t m_cell_size = 4;
  static constexpr std::uint32_t vertices_per_quad = 6;

  // Vertices
  sf::VertexArray m_vertices;
};

}  // namespace carllib

#endif  // CARLLIB_CELL_GRID_HPP
