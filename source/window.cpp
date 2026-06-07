//
// Created by jorge on 01/06/2026.
//

#include "carllib/graphics/window.hpp"

#include <toml++/toml.hpp>

namespace carllib::graphics
{

auto window::create_from_config(std::string_view config_file_name)
    -> std::optional<window> {
  // Open config file
  spdlog::info("Loading config file: {}", config_file_name);
  auto config_file = toml::table();
  try {
    config_file = toml::parse_file(config_file_name);
  } catch (const toml::parse_error& error) {
    spdlog::error("Error while parsing config file: {}. Error: {}",
                  config_file_name,
                  error.description());
    return std::nullopt;
  }

  // Read values from config file
  auto const title = config_file["title"].value_or<std::string>("");
  auto name = config_file["name"].value_or<std::string>("carlib");
  auto const width = config_file["Window"]["width"].value_or<uint32_t>(800);
  auto const height = config_file["Window"]["height"].value_or<uint32_t>(800);

  // OpenGL context settings
  auto context = sf::ContextSettings {};
  context.depthBits = 24;
  context.sRgbCapable = true;

  // Create render target
  spdlog::info("Creating Window");
  auto render_window = sf::RenderWindow(
      sf::VideoMode({width, height}), title, sf::Style::Titlebar | sf::Style::Close , sf::State::Windowed, context);
  if (!render_window.setActive(true)) {
    spdlog::error("Couldn't set main active window");
    return std::nullopt;
  }

  auto main_window = window(std::move(render_window), name);
  return std::make_optional<window>(std::move(main_window));
}

auto window::start_loop() -> int {
  // Main loop
  while (m_is_running) {
    // Handle messages
    while (const auto event = m_render_window.pollEvent()) {
      // Close the window
      if (event->is<sf::Event::Closed>()) {
        m_logger.info("Window is being closed");
        m_is_running = false;
      } else {
        if (m_handle_event_function) m_handle_event_function.value()(*event);
      }
    }

    // Handle drawing
    m_render_window.clear(sf::Color::Black);
    draw();
    m_render_window.display();
  }

  return 0;
}

void window::stop_loop() {
  m_logger.info("Closing Window");
  m_is_running = false;
}

void window::draw() {
  if (m_draw_function) {
    (*m_draw_function)(m_render_window);
  }
}

window::window(sf::RenderWindow&& render_window, std::string name)
    : m_render_window(std::move(render_window))
    , m_logger(name)
    , m_name(std::move(name)) {}

}  // namespace carllib
