/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025 Jan 5
Last update: 2026 Jun 12 */

#ifndef LOG_HPP
#define LOG_HPP

#include <cstdlib> // for exit()
#include <iostream>

class Log {
public:
  enum class LogLevel {
    Debug, // weird prefix and lowercase to prevent name clashes with macros
           // from Linux libraries
    Info,
    Warn,
    Error,
    Fatal
  };

  template <typename T> static void debug(const T &message) {
    if (!p_shouldLog(LogLevel::Debug)) {
      return;
    }

    std::cout << "\x1b[1m"
              << "\x1b[33m" /* Bold Yellow */
              << "(DEBUG):"
              << "\x1b[0m " << message << "\n";
  }
  template <typename T, typename... Args>
  static void debug(const T &message, const Args &...args) {
    if (!p_shouldLog(LogLevel::Debug)) {
      return;
    }

    std::cout << "\x1b[1m"
              << "\x1b[33m" /* Bold Yellow */
              << "(DEBUG):"
              << "\x1b[0m " << message;
    print(args...);
    print("\n");
  }

  template <typename T> static void info(const T &message) {
    if (!p_shouldLog(LogLevel::Info)) {
      return;
    }

    std::cout << "\x1b[32m" /* Green */
              << "(INFO):"
              << "\x1b[0m " << message << "\n";
  }

  template <typename T, typename... Args>
  static void info(const T &message, const Args &...args) {
    if (!p_shouldLog(LogLevel::Info)) {
      return;
    }

    std::cout << "\x1b[32m" /* Green */
              << "(INFO):"
              << "\x1b[0m " << message;

    print(args...);
    print("\n");
  }

  template <typename T> static void warn(const T &message) {
    if (!p_shouldLog(LogLevel::Warn)) {
      return;
    }

    std::cout << "\x1b[33m" /* Yellow */
              << "(WARNING):"
              << "\x1b[0m " << message << "\n";
  }

  template <typename T, typename... Args>
  static void warn(const T &message, const Args &...args) {
    if (!p_shouldLog(LogLevel::Warn)) {
      return;
    }

    std::cout << "\x1b[33m" /* Yellow */
              << "(WARNING):"
              << "\x1b[0m " << message;
    print(args...);
    print("\n");
  }

  template <typename T> static void error(bool terminate, const T &message) {
    if (!p_shouldLog(LogLevel::Error)) {
      return;
    }

    std::cout << "\x1b[31m" /* Red */
              << "(ERROR):"
              << "\x1b[0m " << message << "\n";
    if (terminate) {
      exit(EXIT_FAILURE);
    }
  }

  template <typename T, typename... Args>
  static void error(bool terminate, const T &message, const Args &...args) {
    if (!p_shouldLog(LogLevel::Error)) {
      return;
    }

    std::cout << "\x1b[31m" /* Red */
              << "(ERROR):"
              << "\x1b[0m " << message;
    print(args...);
    print("\n");
    if (terminate) {
      exit(EXIT_FAILURE);
    }
  }

  static void setLogLevel(const LogLevel &level) {
    p_current_log_level = level;
  }

private:
  template <typename T> static void print(const T &value) {
    std::cout << value;
  }

  template <typename T, typename... Args>
  static void print(const T &value, const Args &...args) {
    std::cout << value;
    print(args...);
  }

  static bool p_shouldLog(const LogLevel &log_level);

  inline static LogLevel p_current_log_level = LogLevel::Info;
};

// I'm sorry for this abomination :3
inline bool operator>=(Log::LogLevel lhs, Log::LogLevel rhs) {
  return static_cast<int>(lhs) >= static_cast<int>(rhs);
}

inline bool Log::p_shouldLog(const LogLevel &log_level) {
  // hierarchy: error > warning > info > debug
  return (log_level >= p_current_log_level);
}

#endif // log.hpp