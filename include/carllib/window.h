//
// Created by jorge on 01/06/2026.
//

#ifndef CARLLIB_WINDOW_H
#define CARLLIB_WINDOW_H
#include <optional>
#include <string_view>

#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace carllib
{

// Definition for draw function
using draw_function = std::function<void(sf::RenderWindow&)>;

// Definition for handle key press function
using key_press_function = std::function<void(const sf::Event::KeyPressed&)>;
using key_release_function = std::function<void(const sf::Event::KeyReleased&)>;

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
  static auto create_from_config(std::string_view config_file_name = "init.toml")
      -> std::optional<window>;

  /**
   * Starts the main loop
   * @return Exit code
   */
  auto start_loop() -> int;

  // Copy / Move operations
  window(const window& other) = delete;
  window(window&& other) noexcept = default;
  window& operator=(const window& other) = delete;
  window& operator=(window&& other) noexcept = default;

  // Destructor
  ~window()= default;

  /// Set main functions
  void set_draw_function(draw_function function){ m_draw_function = function; }
  void set_key_press_function(key_press_function function){ m_key_press_function = function; }
  void set_key_release_function(key_release_function function){ m_key_release_function = function; }

  /**
   * Stops the main loop and exits.
   */
  void stop_loop();
private:
  /**
   * Stops the main loop and exits
   */
  void draw();
  void handle_input();

  /**
   * Private constructor
   */
  explicit window(sf::RenderWindow&& render_window, std::string name = "carllib_window");

  // Main window
  sf::RenderWindow m_render_window;

  // Logger
  spdlog::logger m_logger;

  // State
  std::string m_name;
  bool m_is_running = true;

  /// Functional
  std::optional<draw_function> m_draw_function;
  std::optional<key_press_function> m_key_press_function;
  std::optional<key_release_function> m_key_release_function;
};

}  // namespace carllib

#endif  // CARLLIB_WINDOW_H
