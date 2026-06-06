//
// Created by jorge on 03/06/2026.
//

#include <cstddef>
#include <random>

#include "carllib/cell_grid.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace carllib
{

cell_grid::cell_grid(sf::Vector2u initial_size, std::uint32_t cell_size) {
  m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  resize(initial_size, cell_size);
}

void cell_grid::resize(sf::Vector2u new_size, std::uint32_t cell_size) {
  // Resize buffer
  const auto total_vertices = new_size.x * new_size.y * vertices_per_quad;
  m_vertices.resize(total_vertices);

  // Set position of each vertex
  const auto float_cell_size = static_cast<float>(cell_size);
  for (auto col = 0U; col < new_size.x; ++col) {
    for (auto row = 0U; row < new_size.y; ++row) {
      const auto vertex_idx = (col + (row * new_size.x)) * vertices_per_quad;
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

  // Set random colors. std::uniform_int_distribution is only defined for
  // short/int/long/long long (and unsigned variants); 8-bit types are not
  // permitted, so draw an unsigned int in [0, 255] and narrow to uint8_t.
  auto random_generator = std::default_random_engine {};
  auto uniform_generator = std::uniform_int_distribution<unsigned int>(0, 255);
  auto next_channel = [&]
  { return static_cast<std::uint8_t>(uniform_generator(random_generator)); };

  for (auto idx = 0U; idx < m_vertices.getVertexCount(); ++idx) {
    m_vertices[idx].color =
        sf::Color {next_channel(), next_channel(), next_channel()};
  }

  m_size = new_size;
  m_cell_size = cell_size;
}

void cell_grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(m_vertices, states);
}

}  // namespace carllib
