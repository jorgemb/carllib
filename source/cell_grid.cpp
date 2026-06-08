//
// Created by jorge on 03/06/2026.
//

#include <cstddef>
#include <random>

#include "carllib/graphics/cell_grid.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <spdlog/spdlog.h>

namespace carllib::graphics
{

cell_grid::cell_grid(const sf::Vector2u initial_size,
                     const std::uint32_t cell_size) {
  m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  resize(initial_size, cell_size, /*randomize_colors=*/true);
}

auto cell_grid::resize(sf::Vector2u new_size,
                       const std::uint32_t cell_size,
                       const bool randomize_colors) -> bool {
  // Checks
  if (const auto total_size =
          static_cast<std::uint64_t>(new_size.x) * new_size.y;
      total_size == 0 || total_size > std::numeric_limits<size_t>::max())
  {
    spdlog::error("Cannot resize cell_grid with dimensions {}x{}",
                  new_size.x,
                  new_size.y);
    return false;
  }

  // Resize buffer
  const auto total_vertices = new_size.x * new_size.y * vertices_per_quad;
  m_vertices.resize(total_vertices);
  m_size = new_size;
  m_cell_size = cell_size;

  // Set position of each vertex
  const auto float_cell_size = static_cast<float>(cell_size);
  for (auto col = 0U; col < new_size.x; ++col) {
    for (auto row = 0U; row < new_size.y; ++row) {
      const auto vertex_idx = get_first_vertex_of_position({col, row});
      assert(vertex_idx <= m_vertices.getVertexCount() - vertices_per_quad);
      sf::Vertex* tri = &m_vertices[vertex_idx];

      const auto fcol = static_cast<float>(col);
      const auto frow = static_cast<float>(row);

      // Triangle 1 (top-left, top-right, bottom-left)
      tri[0].position =
          sf::Vector2f(fcol * float_cell_size, frow * float_cell_size);
      tri[1].position =
          sf::Vector2f((fcol + 1) * float_cell_size, frow * float_cell_size);
      tri[2].position =
          sf::Vector2f(fcol * float_cell_size, (frow + 1) * float_cell_size);

      // Triangle 2 (top-right, bottom-right, bottom-left)
      tri[3].position =
          sf::Vector2f((fcol + 1) * float_cell_size, frow * float_cell_size);
      tri[4].position = sf::Vector2f((fcol + 1) * float_cell_size,
                                     (frow + 1) * float_cell_size);
      tri[5].position =
          sf::Vector2f(fcol * float_cell_size, (frow + 1) * float_cell_size);
    }
  }

  if (randomize_colors) {
    // Set random colors
    auto random_generator =
        std::default_random_engine {std::random_device {}()};
    auto uniform_generator =
        std::uniform_int_distribution<unsigned int>(0, 255);
    auto next_channel = [&]() -> std::uint8_t
    { return static_cast<std::uint8_t>(uniform_generator(random_generator)); };

    for (auto idx = 0U; idx < m_vertices.getVertexCount(); ++idx) {
      m_vertices[idx].color =
          sf::Color {next_channel(), next_channel(), next_channel()};
    }
  }

  return true;
}

auto cell_grid::set_color(const sf::Vector2u& position, sf::Color color)
    -> bool {
  // Validate the position
  if (position.x >= m_size.x || position.y >= m_size.y) {
    return false;
  }

  // Change the color of the given vertices
  const auto vertex_index = get_first_vertex_of_position(position);
  for (auto i = 0U; i < vertices_per_quad; ++i) {
    m_vertices[vertex_index + i].color = color;
  }

  return true;
}

auto cell_grid::get_color(const sf::Vector2u& position)
    -> std::optional<sf::Color> {
  // Validate the position
  if (position.x >= m_size.x || position.y >= m_size.y) {
    return std::nullopt;
  }

  // Get the color (assume the quad has the same color)
  auto const vertex_index = get_first_vertex_of_position(position);
  return std::make_optional(m_vertices[vertex_index].color);
}

void cell_grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(m_vertices, states);
}

}  // namespace carllib::graphics
