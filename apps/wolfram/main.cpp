///
/// Implements the Wolfram essential Cellular Automata
///

#include <ranges>

#include <CLI/CLI.hpp>
#include <carllib/graphics/window.hpp>

#include "carllib/ca/base_1d.hpp"
#include "carllib/ca/wolfram.hpp"
#include "carllib/graphics/cell_grid.hpp"

using namespace carllib;

namespace
{
class wolfram_window : public graphics::window {
public:
  explicit wolfram_window(ca::wolfram_number rule_number,
                          ca::initial_condition initial_condition,
                          std::uint8_t zoom,
                          std::string_view config_file = "init.toml")
      : window(graphics::create_from_config(config_file))
      , m_zoom(zoom)
      , m_rule_number(rule_number)
      , m_initial_condition(initial_condition) {}

  void on_setup() override {
    // Load font
    m_font = sf::Font("external/42dotSans.ttf");

    // Create and initialize ca
    m_wolfram_ca = std::make_optional<ca::wolfram>(
        m_rule_number, m_rule_width, m_initial_condition);

    // Create text
    m_display_text = std::make_optional<sf::Text>(m_font);
  }

  void on_draw() override {
    m_render_window.clear(sf::Color::Magenta);

    // Draw grid
    m_render_window.draw(m_grid);

    // Draw text
    m_display_text->setFillColor(sf::Color::Black);
    m_display_text->setStyle(sf::Text::Style::Bold);
    m_render_window.draw(*m_display_text);

    m_render_window.display();
  }

  void on_logic() override {
    // Calculate current width and height according to zoom level
    const auto window_size = m_render_window.getSize();
    auto width = static_cast<std::uint32_t>(
        std::ceil(window_size.x / static_cast<double>(m_zoom)));
    auto height = static_cast<std::uint32_t>(
        std::ceil(window_size.y / static_cast<double>(m_zoom)));

    // .. calculate missing generations
    while (m_wolfram_ca->total_generations() < height) {
      m_wolfram_ca->calculate_next_generation();
    }

    // .. resize grid
    if (m_grid.size() != sf::Vector2u {width, height}) {
      m_grid.resize({width, height}, static_cast<std::uint32_t>(m_zoom), true);
    }

    // .. fill in the grid
    for (auto row = 0U; row < m_wolfram_ca->total_generations(); ++row) {
      for (auto const [col, value] : std::views::enumerate(
               m_wolfram_ca->get_generation_data(row)
               | std::views::drop((m_wolfram_ca->width() - width) / 2)))
      {
        const auto color = value ? sf::Color::Black : sf::Color::White;
        m_grid.set_color_at({static_cast<unsigned>(col), row}, color);
      }
    }

    // .. set text
    m_display_text->setString(
        fmt::format("Size: {}x{} - Zoom: {}", width, height, m_zoom));
  }

  void on_handle_event(const sf::Event& event) override {
    if (auto wheel_scroll_event = event.getIf<sf::Event::MouseWheelScrolled>())
    {
      const auto factor =
          static_cast<std::int32_t>(std::floor(wheel_scroll_event->delta));
      m_zoom = std::clamp(m_zoom + factor, 1, 32);
    } else {
      window::on_handle_event(event);
    }
  }

private:
  sf::Font m_font;
  graphics::cell_grid m_grid;

  int m_zoom = 4;
  std::optional<sf::Text> m_display_text;

  ca::wolfram_number m_rule_number {90};
  ca::initial_condition m_initial_condition;
  const std::size_t m_rule_width = 4000;
  std::optional<ca::wolfram> m_wolfram_ca;
};
}  // namespace

auto main(int argc, char* argv[]) -> int {
  // Parse arguments
  auto app = CLI::App {"Calculate Wolfram essential Cellular Automata"};
  argv = app.ensure_utf8(argv);

  auto rule_number = std::uint8_t {};
  app.add_option("-r, --rule", rule_number, "Rule number to use")->required();

  auto zoom = std::uint8_t {4};
  app.add_option("-z, --zoom", zoom, "Initial zoom value")->default_val(4);

  auto random = false;
  app.add_flag("--random", random, "Use random initial condition");


  auto config_file = std::string {"init.toml"};
  app.add_option("-c,--config", config_file, "Path to config file")
      ->check(CLI::ExistingFile);
  CLI11_PARSE(app, argc, argv);

  // Check if random initial condition
  auto initial_condition =
      random ? ca::initial_condition::random : ca::initial_condition::standard;

  // Start main window
  auto main_window = wolfram_window(
      ca::wolfram_number {rule_number}, initial_condition, zoom, config_file);

  return main_window.start_loop();
}
