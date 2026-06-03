#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "carllib/cell_grid.hpp"
#include "carllib/window.hpp"

auto main() -> int {
  auto main_window = carllib::window::create_from_config();
  if (!main_window) {
    spdlog::error("Couldn't create new window");
    return -1;
  }

  auto grid = carllib::cell_grid({200, 200}, 4);
  auto zoom = 1.0f;

  // Add functions
  main_window->set_draw_function(
      [&grid, &zoom](sf::RenderWindow& render_window) -> void
      {
        // Set zoom
        auto view = render_window.getDefaultView();
        view.zoom(zoom);
        render_window.setView(view);

        // Draw grid
        render_window.draw(grid);

        // Draw current zoom
      });

  main_window->set_handle_event_function(
      [&zoom](const sf::Event event)
      {
        // Wheel scroll
        if (auto wheel_scroll_event = event.getIf<sf::Event::MouseWheelScrolled>()) {
          const auto factor = wheel_scroll_event->delta > 0 ? -0.1f : 0.1f;
          zoom = std::clamp(zoom + factor, 0.1f, 1.2f);
        }
      });

  return main_window->start_loop();
}
