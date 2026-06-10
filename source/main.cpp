#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "carllib/ca/linear_toroidal.hpp"
#include "carllib/graphics/cell_grid.hpp"
#include "carllib/graphics/window.hpp"

auto main() -> int {
  auto main_window = carllib::graphics::window::create_from_config();
  if (!main_window) {
    spdlog::error("Couldn't create new window");
    return -1;
  }

  auto font = sf::Font("42dotSans.ttf");
  auto grid = carllib::graphics::cell_grid({10, 10}, 1);
  auto zoom = 1;
  auto zoom_text = sf::Text {font};

  auto ca_line = carllib::ca::linear_toroidal<bool> {false, false, true, false, false};

  // Add functions
  main_window->set_draw_function(
      [&grid, &zoom, &zoom_text, &ca_line](
          sf::RenderWindow& render_window) -> void
      {
        // Draw grid
        render_window.draw(grid);

        // Draw text
        const auto window_size = render_window.getSize();
        auto width = static_cast<std::uint32_t>(
            std::ceil(window_size.x / static_cast<double>(zoom)));
        auto height = static_cast<std::uint32_t>(
            std::ceil(window_size.y / static_cast<double>(zoom)));

        // .. check if a resize is necessary
        if (grid.size() != sf::Vector2u {width, height}) {
          // Grid requires resizing
          grid.resize({width, height},
                      static_cast<std::uint32_t>(zoom),
                      /*randomize_colors=*/true);
        }

        // Fill in the grid cell
        for (auto row = 0U; row < ca_line.total_generations(); ++row) {
          for (auto const &col: ca_line.get_generation_data(row)) {
            const auto color = col
                ? sf::Color::Black
                : sf::Color::White;
            grid.set_color_at({row, col}, color);
          }
        }

        // Set zoom text
        zoom_text.setString(
            fmt::format("Size: {}x{} - Zoom: {}", width, height, zoom));
        zoom_text.setFillColor(sf::Color::Black);
        zoom_text.setStyle(sf::Text::Style::Bold);
        render_window.draw(zoom_text);
      });

  main_window->set_handle_event_function(
      [&zoom](sf::RenderWindow& window, const sf::Event& event)
      {
        // Wheel scroll
        if (auto wheel_scroll_event =
                event.getIf<sf::Event::MouseWheelScrolled>())
        {
          const auto factor =
              static_cast<std::int32_t>(std::floor(wheel_scroll_event->delta));
          zoom = std::clamp(zoom + factor, 1, 32);
        } else if (const auto resize_event =
                       event.getIf<sf::Event::Resized>()) {
          // Update the view to match the new window size
          const sf::FloatRect visibleArea(
              {0.f, 0.f},
              {static_cast<float>(resize_event->size.x),
               static_cast<float>(resize_event->size.y)});
          window.setView(sf::View(visibleArea));
        }
      });

  return main_window->start_loop();
}
