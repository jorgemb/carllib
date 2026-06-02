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

  return main_window->start_loop();

  // Try to read the file name
  // if (argc <= 1) {
  //   fmt::print("Usage: {} <CONFIG_FILE>", argv[0]);
  //   return -1;
  // }
  // auto config_file = toml::table();
  // try {
  //   config_file = toml::parse_file(argv[1]);
  // } catch (const toml::parse_error& error) {
  //   spdlog::error(fmt::format("Error while parsing file {}. Error: {}", argv[1], error.what()));
  //   return -1;
  // }
  //
  // // Retrieve config values
  // auto title = config_file["title"].value_or<std::string>("");
  // auto width = config_file["Window"]["width"].value_or<uint32_t>(800);
  // auto height = config_file["Window"]["height"].value_or<uint32_t>(800);
  //
  // // Create window
  // auto main_window = sf::RenderWindow(sf::VideoMode({width, height}), title);
  //
  // while (main_window.isOpen()) {
  //   // Poll for events
  //   while (const auto event = main_window.pollEvent()) {
  //     // Check type of event
  //     if (event->is<sf::Event::Closed>()) {
  //       main_window.close();
  //     }
  //   }
  //
  //   // Clear the window
  //   main_window.clear(sf::Color::Black);
  //
  //   // Draw the window
  //   main_window.display();
  // }

  return 0;
}
