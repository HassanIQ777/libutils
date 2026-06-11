/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025 Jun 11
Last update: 2026 Jun 12 */

#pragma once

#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdint.h>
#include <string>

class Input {
public:
  // This uses std::cin
  template <typename T> static std::optional<T> read() {
    T inp;
    std::cin >> inp;
    if (std::cin.fail()) {
      return resetcin<T>();
    }
    return inp;
  }

  // This uses std::cin
  template <typename T> static std::optional<T> read(const std::string &label) {
    std::cout << label;
    T inp;
    std::cin >> inp;
    if (std::cin.fail()) {
      return resetcin<T>();
    }
    return inp;
  }

  template <typename T> static std::optional<T> readline() {
    std::string inp;
    if (!std::getline(std::cin, inp))
      return resetcin<T>();

    // std::string is a special snowflake — skip the stream dance
    if constexpr (std::is_same_v<T, std::string>)
      return inp;

    std::istringstream ss(inp);
    T value;

    if (!(ss >> value))
      return std::nullopt;

    return value;
  }

private:
  template <typename T> static std::optional<T> resetcin() {
    std::cin.clear(); // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return std::nullopt;
  }
};