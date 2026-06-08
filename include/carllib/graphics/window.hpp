//
// Created by jorge on 01/06/2026.
//

#ifndef CARLLIB_WINDOW_H
#define CARLLIB_WINDOW_H
#include <optional>
#include <string_view>

#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace carllib::graphics
{

// Definition for draw function
using draw_function = std::function<void(sf::RenderWindow&)>;
using handle_event_function = std::function<void(sf::RenderWindow&, const sf::Event)>;

/**
 * Represents a Window with a main loop.
 */
class window {
public:
  /**
   * Creates a new window from the configuration file
   * @param config_file_name C
   * @return
   */
  static auto create_from_config(
      std::string_view config_file_name = "init.toml") -> std::optional<window>;

  /**
   * Starts the main loop
   * @return Exit code
   */
  auto start_loop() -> int;

  // Copy / Move operations
  window(const window& other) = delete;
  window(window&& other) noexcept = default;
  auto operator=(const window& other) -> window& = delete;
  auto operator=(window&& other) noexcept -> window& = default;

  // Destructor
  ~window() = default;

  /// Set main functions
  void set_draw_function(const draw_function& function) {
    m_draw_function = function;
  }

  void set_handle_event_function(const handle_event_function& function) {
    m_handle_event_function = function;
  }

  /// Get values
  [[nodiscard]] auto size() const -> sf::Vector2u {
    return m_render_window.getSize();
  }

  /**
   * Stops the main loop and exits.
   */
  void stop_loop();

private:
  /**
   * Stops the main loop and exits
   */
  void draw();

  /**
   * Private constructor
   */
  explicit window(sf::RenderWindow&& render_window);

  // Main window
  sf::RenderWindow m_render_window;

  // Logger
  std::shared_ptr<spdlog::logger> m_logger;

  // State
  bool m_is_running = true;

  /// Functional
  std::optional<draw_function> m_draw_function;
  std::optional<handle_event_function> m_handle_event_function;
};

}  // namespace carllib::graphics

#endif  // CARLLIB_WINDOW_H
