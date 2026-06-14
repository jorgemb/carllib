//
// Created by jorge on 01/06/2026.
//

#ifndef CARLLIB_WINDOW_H
#define CARLLIB_WINDOW_H
#include <memory>
#include <string_view>

#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace carllib::graphics
{
/** Creates a new window from the configuration file
 * @param config_file_name C
 * @return
 */
auto create_from_config(std::string_view config_file_name = "init.toml")
    -> sf::RenderWindow;

/**
 * Represents a Window with a main loop.
 */
class window {
public:
  /**
   *
   * Starts the main loop
   * @return Exit code
   */
  auto start_loop() -> int;

  /**
   * Stops the main loop and exits.
   */
  void stop_loop();

  // Copy / Move operations
  window(const window& other) = delete;
  window(window&& other) noexcept = default;
  auto operator=(const window& other) -> window& = delete;
  auto operator=(window&& other) noexcept -> window& = default;

  // Destructor
  virtual ~window() = default;

  /**
   * Called at the beginning of the window, before starting loop
   */
  virtual void on_setup() {}

  /**
   * Called after end of loop
   */
  virtual void on_teardown() {}

  /**
   * Handle draw logic towards the render target
   */
  virtual void on_draw();

  /**
   * Perform non-draw logic
   */
  virtual void on_logic() {}

  /**
   * Handle the given event
   * @param event
   */
  virtual void on_handle_event(const sf::Event& event);

  /// Get values
  [[nodiscard]] auto size() const -> sf::Vector2u {
    return m_render_window.getSize();
  }

protected:
  /**
   * Constructor
   */
  explicit window(sf::RenderWindow&& render_window);

  // Main window
  sf::RenderWindow m_render_window;

  // State
  bool m_is_running = true;
};

}  // namespace carllib::graphics

#endif  // CARLLIB_WINDOW_H
