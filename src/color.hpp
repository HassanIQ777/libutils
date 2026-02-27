/* Part of https://github.com/HassanIQ777/libutils
Made on 2024 Dec 2 */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>
#include <iostream>

namespace color
{
// Modifiers

inline constexpr const char *_RESET = "\x1b[0m";
inline constexpr const char *_BOLD = "\x1b[1m";
inline constexpr const char *_ITALIC = "\x1b[3m";
inline constexpr const char *_UNDER_LINE = "\x1b[4m";
inline constexpr const char *_STRIKE_THROUGH = "\x1b[9m";

// Text colors

inline constexpr const char *TXT_BLACK = "\x1b[30m";
inline constexpr const char *TXT_RED = "\x1b[31m";
inline constexpr const char *TXT_GREEN = "\x1b[32m";
inline constexpr const char *TXT_YELLOW = "\x1b[33m";
inline constexpr const char *TXT_BLUE = "\x1b[34m";
inline constexpr const char *TXT_MAGENTA = "\x1b[35m";
inline constexpr const char *TXT_CYAN = "\x1b[36m";
inline constexpr const char *TXT_WHITE = "\x1b[37m";

// Background colors

inline constexpr const char *BG_BLACK = "\x1b[40m";
inline constexpr const char *BG_RED = "\x1b[41m";
inline constexpr const char *BG_GREEN = "\x1b[42m";
inline constexpr const char *BG_YELLOW = "\x1b[43m";
inline constexpr const char *BG_BLUE = "\x1b[44m";
inline constexpr const char *BG_MAGENTA = "\x1b[45m";
inline constexpr const char *BG_CYAN = "\x1b[46m";
inline constexpr const char *BG_WHITE = "\x1b[47m";

// Advanced features

inline std::string fg_rgb(int r, int g, int b)
{
	return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

inline std::string bg_rgb(int r, int g, int b)
{
	return "\x1b[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
} // example: color::bg_rgb(100,100,100) gray background

inline std::string style(const std::string &modifier, const std::string &text_color, const std::string &background_color = "")
{
	return modifier + text_color + background_color;
} // example: std:: cout << color::style(color::modifier::BOLD,color::text::YELLOW,color::background::WHITE) << "Hello World!"<<color::modifier::RESET << std::endl;
} // namespace color

#endif // COLOR_HPP
