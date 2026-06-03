#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "carllib/window.h"

auto main(const int argc, char* argv[]) -> int {
  auto main_window = carllib::window::create_from_config();
  if (!main_window) {
    spdlog::error("Couldn't create new window");
    return -1;
  }

  // Add functions
  main_window->set_draw_function([](sf::RenderWindow& render_window)
  -> void {
    auto circle = sf::CircleShape(50.f);
    circle.setFillColor(sf::Color::Magenta);

    render_window.draw(circle);
  });

  main_window->set_key_press_function([&main_window](auto key_press)
  {
  });

  return main_window->start_loop();
}
