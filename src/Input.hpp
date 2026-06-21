/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025 Jun 11
Last update: 2026 Jun 21 */

#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdint.h>
#include <string>
#include <type_traits>

#ifdef _WIN32
#include <conio.h> // _getch()

#else

#include <termios.h> // termios struct, tcgetattr, tcsetattr, TCSANOW
#include <unistd.h>  // STDIN_FILENO

#endif

class Input {
public:
  // This uses std::cin
  template <typename T> static std::optional<T> read() {
    T inp;
    std::cin >> inp;
    if (std::cin.fail())
      return resetcin<T>();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // evict the ghost newline
    return inp;
  }

  // This uses std::cin
  template <typename T> static std::optional<T> read(const std::string &label) {
    if (!label.empty())
      std::cout << label;
    T inp;
    std::cin >> inp;
    if (std::cin.fail())
      return resetcin<T>();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // evict the ghost newline
    return inp;
  }

  // This uses std::getline
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

  // This uses std::getline
  template <typename T>
  static std::optional<T> readline(const std::string &label) {
    std::cout << label;
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

  template <typename T>
  static std::optional<T> readInRange(T min, T max,
                                      const std::string &label = "") {
    static_assert(std::is_arithmetic_v<T>,
                  "ranges only make sense for numbers, chief");
    auto val = readline<T>(label);
    if (!val || *val < min || *val > max)
      return std::nullopt;
    return val;
  }

  template <typename T>
  static T
  readUntilValid(const std::string &label = "",
                 const std::string &errMsg = "Invalid input, try again: ") {
    while (true) {
      auto val = readline<T>(label);
      if (val)
        return *val;
      std::cout << errMsg;
    }
  }

  static std::optional<char> readChar(const std::string &label = "") {
    if (!label.empty())
      std::cout << label;
    char c;
    if (!std::cin.get(c))
      return resetcin<char>();
    return c;
  }

  template <typename T>
  static std::optional<T> readOneOf(const std::initializer_list<T> &choices,
                                    const std::string &label = "") {
    auto val = readline<T>(label);
    if (!val)
      return std::nullopt;
    for (const auto &c : choices)
      if (*val == c)
        return val;
    return std::nullopt;
  }

  template <typename T>
  static T readWithDefault(const T &def, const std::string &label = "") {
    if (!label.empty())
      std::cout << label;
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
      return def;

    std::istringstream ss(line);
    T val;
    if (!(ss >> val))
      return def;
    return val;
  }

  static std::string readSilent(const std::string &label = "Password: ") {
    std::cout << label;
    std::string pass;
#ifdef _WIN32
    char c;
    while ((c = _getch()) != '\r')
      pass += c;
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, pass);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    std::cout << '\n';
    return pass;
  }

  // customizable
  template <typename T>
  static std::optional<T>
  readParsed(std::function<std::optional<T>(const std::string &)> parser,
             const std::string &label = "") {
    auto line = readline<std::string>(label);
    if (!line)
      return std::nullopt;
    return parser(*line);
  }

private:
  template <typename T> static std::optional<T> resetcin() {
    std::cin.clear(); // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return std::nullopt;
  }
};
