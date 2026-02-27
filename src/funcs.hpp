/* Part of https://github.com/HassanIQ777/libutils
Made on    : 2024 Nov 17
Last update: 2025 Nov 06 */

#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace funcs {

template <typename T>
void printTimed(T text, int cd = 0, int end_cd = 0);

template <typename T>
void printCentered(T text, int cd = 0, int end_cd = 0);

template <typename T>
void printRight(T text, int cd = 0, int end_cd = 0);

inline void printLeftMiddleRight(const std::string &left,
                                 const std::string &middle = "",
                                 const std::string &right = "");

template <typename T>
void print(const T &value);

template <typename T, typename... Args>
void print(const T &value, const Args &...args);

template <typename T>
std::string str(const T &n);

inline std::string lowercase(std::string text);
inline std::string uppercase(std::string text);
inline void removeChar(std::string &text, char char_to_remove);
inline void replaceChar(std::string &text, char old_char, char new_char);

inline size_t getTerminalWidth();
inline std::string getPlatform();
inline void clearTerminal();
inline std::string currentTime();
inline void msleep(size_t milliseconds);
inline std::string getKeyPress();

inline bool hasSequence(const std::string &text, const std::string &sequence);
inline bool isNumber(const std::string &s);
inline std::vector<std::string> split(const std::string &text, char delimiter);

// Printing utilities
template <typename T>
void printTimed(T text, int cd, int end_cd) {
  const std::string string_text = funcs::str(text);
  if (cd == 0) {
    std::cout << string_text;
    return;
  }

  for (char ch : string_text) {
    std::this_thread::sleep_for(std::chrono::milliseconds(cd));
    std::cout << ch;
    std::cout.flush();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(end_cd));
  std::cout.flush();
}

template <typename T>
void printCentered(T text, int cd, int end_cd) {
  std::string string_text = str(text);
  const size_t width = getTerminalWidth();
  if (string_text.length() > width) {
    string_text = string_text.substr(0, width - 1);
  }

  const size_t padding = (width - string_text.size()) / 2;
  std::cout << std::string(padding, ' ');
  printTimed(string_text, cd, end_cd);
}

template <typename T>
void printRight(T text, int cd, int end_cd) {
  std::string string_text = str(text);
  const size_t width = getTerminalWidth();
  if (string_text.length() > width) {
    string_text = string_text.substr(0, width - 1);
  }

  const size_t padding = width - string_text.size();
  std::cout << std::string(padding, ' ');
  printTimed(string_text, cd, end_cd);
}

inline void printLeftMiddleRight(const std::string &left,
                                 const std::string &middle,
                                 const std::string &right) {
  const size_t width = funcs::getTerminalWidth();
  const size_t max_left = width / 3;
  const size_t max_middle = width / 3;
  const size_t max_right = width - (max_left + max_middle);

  const std::string l = left.substr(0, max_left);
  const std::string m = middle.substr(0, max_middle);
  const std::string r = right.substr(0, max_right);

  const size_t used = l.size() + m.size() + r.size();
  const size_t remaining = width - used;
  const size_t padding_left = remaining / 2;
  const size_t padding_right = remaining - padding_left;

  std::cout << l << std::string(padding_left, ' ') << m
            << std::string(padding_right, ' ') << r;
}

template <typename T>
void print(const T &value) {
  std::cout << value;
}

template <typename T, typename... Args>
void print(const T &value, const Args &...args) {
  std::cout << value;
  print(args...);
}

template <typename T>
std::string str(const T &n) {
  std::ostringstream stm;
  stm << n;
  return stm.str();
}

// String utilities
inline std::string lowercase(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return text;
}

inline std::string uppercase(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return text;
}

// Terminal and platform utilities
inline size_t getTerminalWidth() {
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdOut == INVALID_HANDLE_VALUE) {
    return 80;
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    return 80;
  }

  return static_cast<size_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
#else
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl");
    return 80;
  }

  return static_cast<size_t>(w.ws_col);
#endif
}

inline std::string getPlatform() {
#ifdef _WIN32
  return "Windows";
#else
  struct utsname buffer;
  if (uname(&buffer) == 0) {
    return std::string(buffer.sysname);
  }
  return "Unknown";
#endif
}

inline void clearTerminal() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

inline std::string currentTime() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t time = std::chrono::system_clock::to_time_t(now);
  const std::tm tm = *std::localtime(&time);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

inline void msleep(size_t milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

inline std::string getKeyPress() {
  std::string sequence;

#ifdef _WIN32
  int ch = _getch();
  sequence += static_cast<char>(ch);

  if (ch == 0 || ch == 224) {
    sequence += static_cast<char>(_getch());
    sequence += static_cast<char>(_getch());
  }
#else
  struct termios oldt, newt;
  unsigned char ch;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(static_cast<unsigned int>(ICANON | ECHO));
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  ch = static_cast<unsigned char>(getchar());
  sequence += static_cast<char>(ch);

  if (ch == '\033') {
    sequence += static_cast<char>(static_cast<unsigned char>(getchar()));
    sequence += static_cast<char>(static_cast<unsigned char>(getchar()));
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

  return sequence;
}

inline void removeChar(std::string &text, char char_to_remove) {
  text.erase(std::remove(text.begin(), text.end(), char_to_remove), text.end());
}

inline void replaceChar(std::string &text, char old_char, char new_char) {
  std::replace(text.begin(), text.end(), old_char, new_char);
}

inline bool hasSequence(const std::string &text, const std::string &sequence) {
  return text.find(sequence) != std::string::npos;
}

inline bool isNumber(const std::string &s) {
  try {
    long double parsed = std::stold(s);
    parsed += 0;
  } catch (...) {
    return false;
  }
  return true;
}

inline std::vector<std::string> split(const std::string &text, char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(text);
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    if (!item.empty()) {
      tokens.push_back(item);
    }
  }

  return tokens;
}

template <typename T, typename U, typename V>
constexpr auto clamp(const T &n, const U &lo, const V &hi) {
  using R = std::common_type_t<T, U, V>;
  R nn = n;
  R l = lo;
  R h = hi;

  if (nn < l) {
    return l;
  }
  if (nn > h) {
    return h;
  }
  return nn;
}

inline void alternativeTerminal() {
  std::cout << "\033[?1049h";
}

inline void restoreTerminal() {
  std::cout << "\033[?1049l";
}

} // namespace funcs

inline void funcs_staticAssert_impl(bool expression, const char *file, int line) {
  if (!expression) {
    std::cerr << file << ":" << line << "\n";
    std::exit(EXIT_FAILURE);
  }
}

inline void funcs_staticAssert_impl(bool expression, const std::string &msg,
                                const char *file, int line) {
  if (!expression) {
    std::cerr << file << ":" << line << " -> " << msg << "\n";
    std::exit(EXIT_FAILURE);
  }
}

#define funcs_staticAssert(...)                                                   \
  funcs_staticAssert_dispatch(__VA_ARGS__, funcs_staticAssert2,                   \
                              funcs_staticAssert1)(__VA_ARGS__)
#define funcs_staticAssert_dispatch(_1, _2, NAME, ...) NAME
#define funcs_staticAssert1(expr) funcs_staticAssert_impl(expr, __FILE__, __LINE__)
#define funcs_staticAssert2(expr, msg)                                            \
  funcs_staticAssert_impl(expr, msg, __FILE__, __LINE__)

#endif // FUNCS_HPP
