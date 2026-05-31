#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

auto main(const int argc, char* argv[]) -> int {
  // Try to read the file name
  if (argc <= 1) {
    fmt::print("Usage: {} <CONFIG_FILE>", argv[0]);
    return -1;
  }
  auto config_file = toml::table();
  try {
    config_file = toml::parse_file(argv[1]);
  } catch (const toml::parse_error& error) {
    spdlog::error(fmt::format("Error while parsing file {}. Error: {}", argv[1], error.what()));
    return -1;
  }

  // Retrieve config values
  auto title = config_file["title"].value_or<std::string_view>("");
  auto width = config_file["window"]["width"].value_or<uint16_t>(800);
  auto height = config_file["window"]["height"].value_or<uint16_t>(800);

  fmt::println("{} - {}x{}", title, width, height);
  return 0;
}
