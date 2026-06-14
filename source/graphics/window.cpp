//
// Created by jorge on 01/06/2026.
//

#include "carllib/graphics/window.hpp"

#include <toml++/toml.hpp>

namespace carllib::graphics
{

auto create_from_config(std::string_view config_file_name) -> sf::RenderWindow {
  // Open config file
  spdlog::info("Loading config file: {}", config_file_name);
  auto config_file = toml::table();
  try {
    config_file = toml::parse_file(config_file_name);
  } catch (const toml::parse_error& error) {
    spdlog::error("Error while parsing config file: {}. Error: {}",
                  config_file_name,
                  error.description());
    throw std::runtime_error("Couldn't create window");
  }

  // Read values from config file
  auto const title = config_file["window"]["title"].value_or<std::string>("");
  auto const width = config_file["window"]["width"].value_or<uint32_t>(800);
  auto const height = config_file["window"]["height"].value_or<uint32_t>(800);

  // OpenGL context settings
  auto context = sf::ContextSettings {};
  context.depthBits = 24;
  context.sRgbCapable = true;

  // Create render target
  spdlog::info("Creating Window");
  auto render_window = sf::RenderWindow(sf::VideoMode({width, height}),
                                        title,
                                        sf::Style::Default,
                                        sf::State::Windowed,
                                        context);
  if (!render_window.setActive(true)) {
    spdlog::error("Couldn't set main active window");
    throw std::runtime_error("Couldn't create window");
  }

  return render_window;
}

auto window::start_loop() -> int {
  on_setup();

  // Main loop
  while (m_is_running) {
    // Handle messages
    while (const auto event = m_render_window.pollEvent()) {
      on_handle_event(*event);
    }

    // Handle logic
    on_logic();

    // Handle drawing
    on_draw();
  }

  on_teardown();
  return 0;
}

void window::stop_loop() {
  spdlog::info("Closing Window");
  m_is_running = false;
}

void window::on_draw() {
  m_render_window.clear(sf::Color::Magenta);
  m_render_window.display();
}

void window::on_handle_event(const sf::Event& event) {
  if (event.getIf<sf::Event::Closed>() != nullptr) {
    // Handle exit
    stop_loop();
  } else if (const auto* resize_event = event.getIf<sf::Event::Resized>()) {
    // Update view to match window size
    const sf::FloatRect visibleArea({0.f, 0.f},
                                    {static_cast<float>(resize_event->size.x),
                                     static_cast<float>(resize_event->size.y)});
    m_render_window.setView(sf::View(visibleArea));
  }
}

window::window(sf::RenderWindow&& render_window)
    : m_render_window(std::move(render_window)) {}

}  // namespace carllib::graphics
