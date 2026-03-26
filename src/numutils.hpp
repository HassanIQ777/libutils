/* Part of https://github.com/HassanIQ777/libutils
Made on:     2026 Mar 08
Last update: 2026 Mar 26 */

#ifndef NUMUTILS_HPP
#define NUMUTILS_HPP

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace numutils {

/*
  @ Insert thousands separators
  @ Example: 1234567 -> "1,234,567"
 */
inline std::string groupDigits(long long value, char separator = ',') {
  bool negative = value < 0;
  std::string s = std::to_string(std::llabs(value));

  for (int i = s.length() - 3; i > 0; i -= 3)
    s.insert(i, 1, separator);

  if (negative)
    s.insert(s.begin(), '-');

  return s;
}

/*
   @ Fixed decimal precision
   @ Example: 3.14159 -> "3.14"
 */
template <typename T> inline std::string fixed(T value, int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << value;
  return ss.str();
}

/*
   @ Pad numbers with zeros
   @ Example: 42 -> "00042"
 */
inline std::string zeroPad(long long value, size_t width) {
  std::ostringstream ss;
  ss << std::setw(width) << std::setfill('0') << value;
  return ss.str();
}

/*
   @ Human readable large numbers
   @ Example: 1532000 -> "1.53M"
 */
inline std::string human(double value) {
  const char *suffix[] = {"", "K", "M", "B", "T"};

  int i = 0;
  while (value >= 1000.0 && i < 4) {
    value /= 1000.0;
    i++;
  }

  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2) << value << suffix[i];
  return ss.str();
}

/*
   @ Base conversion
   @ Example: 255 -> "ff"
 */
inline std::string toBase(uint64_t value, int base) {
  static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";

  if (base < 2 || base > 36)
    return "";

  std::string result;

  do {
    result.push_back(digits[value % base]);
    value /= base;
  } while (value);

  std::reverse(result.begin(), result.end());
  return result;
}

/*
   @ Binary formatting
   @ Example: 42 -> "0b101010"
 */
inline std::string binary(uint64_t value, bool prefix = true) {
  if (value == 0)
    return prefix ? "0b0" : "0";

  std::string out;

  while (value) {
    out.push_back((value & 1) ? '1' : '0');
    value >>= 1;
  }

  std::reverse(out.begin(), out.end());

  if (prefix)
    out.insert(0, "0b");

  return out;
}

/*
   @ Hex formatting
   @ Example: 255 -> "0xFF"
 */
inline std::string hex(uint64_t value, bool prefix = true,
                       bool uppercase = true) {
  std::ostringstream ss;

  if (prefix)
    ss << "0x";

  if (uppercase)
    ss << std::uppercase;

  ss << std::hex << value;

  return ss.str();
}

/*
   @ Duration formatting
   @ Example: 3721 -> "1h 2m 1s"
 */
inline std::string duration(uint64_t seconds) {
  uint64_t h = seconds / 3600;
  uint64_t m = (seconds % 3600) / 60;
  uint64_t s = seconds % 60;

  std::string out;

  if (h)
    out += std::to_string(h) + "h ";
  if (m)
    out += std::to_string(m) + "m ";
  if (s || out.empty())
    out += std::to_string(s) + "s";

  if (!out.empty() && out.back() == ' ')
    out.pop_back();

  return out;
}

/*
   @ Byte size formatting
   @ Example: 12345678 -> "11.77 MB"
 */
inline std::string bytes(uint64_t value) {
  const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};

  double size = static_cast<double>(value);
  int unit = 0;

  while (size >= 1024.0 && unit < 5) {
    size /= 1024.0;
    unit++;
  }

  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2) << size << " " << units[unit];
  return ss.str();
}

/*
   @ Percentage formatting
   @ Example: 0.853 -> "85.30%"
 */
inline std::string percent(double value, int precision = 2) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << value * 100 << "%";
  return ss.str();
}

/*
   @ Scientific notation helper
   @ Example: 123456 -> "1.23e+05"
 */
inline std::string scientific(double value, int precision = 2) {
  std::ostringstream ss;
  ss << std::scientific << std::setprecision(precision) << value;
  return ss.str();
}

/*
   @ Fast digit counting
   @ Example: digits(12345) -> 5
 */
inline int digits(uint64_t n) {
  int d = 1;

  while (n >= 10) {
    n /= 10;
    d++;
  }

  return d;
}

/*
  @ ordinal(1)   -> "1st"
  @ ordinal(2)   -> "2nd"
  @ ordinal(3)   -> "3rd"
*/
inline std::string ordinal(uint64_t n) {
  uint64_t mod100 = n % 100;

  std::string suffix = "th";

  if (mod100 < 11 || mod100 > 13) {
    switch (n % 10) {
    case 1:
      suffix = "st";
      break;
    case 2:
      suffix = "nd";
      break;
    case 3:
      suffix = "rd";
      break;
    }
  }

  return std::to_string(n) + suffix;
}

/*
@ time_ms(532)       -> "532ms"
@ time_ms(1534)      -> "1.53s"
*/
inline std::string time_ms(uint64_t ms) {
  std::ostringstream ss;

  if (ms < 1000) {
    ss << ms << "ms";
    return ss.str();
  }

  double seconds = ms / 1000.0;

  if (seconds < 60) {
    ss << std::fixed << std::setprecision(2) << seconds << "s";
    return ss.str();
  }

  uint64_t minutes = ms / 60000;
  uint64_t seconds_rem = (ms % 60000) / 1000;

  ss << minutes << "m " << seconds_rem << "s";
  return ss.str();
}

/*@ align(42,5)           -> "   42"
  @ align(42,5,'0')       -> "00042"
  @ align(42,5,' ',false) -> "42   "*/
template <typename T>
inline std::string align(const T &value, size_t width, char fill = ' ',
                         bool right = true) {
  std::ostringstream ss;

  if (right)
    ss << std::setw(width) << std::setfill(fill) << value;
  else
    ss << std::left << std::setw(width) << std::setfill(fill) << value;

  return ss.str();
}

} // namespace numutils

#endif