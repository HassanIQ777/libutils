/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ make your programs colorful
Made on 2024 Dec 2
*/

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>
#include <iostream>

namespace color
{
// Modifiers

 const char *_RESET          = "\x1b[0m";
 const char *_BOLD           = "\x1b[1m";
 const char *_ITALIC         = "\x1b[3m";
 const char *_UNDER_LINE     = "\x1b[4m";
 const char *_STRIKE_THROUGH = "\x1b[9m";

// Text colors

 const char *TXT_BLACK   = "\x1b[30m";
 const char *TXT_RED     = "\x1b[31m";
 const char *TXT_GREEN   = "\x1b[32m";
 const char *TXT_YELLOW  = "\x1b[33m";
 const char *TXT_BLUE    = "\x1b[34m";
 const char *TXT_MAGENTA = "\x1b[35m";
 const char *TXT_CYAN    = "\x1b[36m";
 const char *TXT_WHITE   = "\x1b[37m";

// Background colors

 const char *BG_BLACK   = "\x1b[40m";
 const char *BG_RED     = "\x1b[41m";
 const char *BG_GREEN   = "\x1b[42m";
 const char *BG_YELLOW  = "\x1b[43m";
 const char *BG_BLUE    = "\x1b[44m";
 const char *BG_MAGENTA = "\x1b[45m";
 const char *BG_CYAN    = "\x1b[46m";
 const char *BG_WHITE   = "\x1b[47m";

// Advanced features

std::string fg_rgb(int r, int g, int b)
{
	return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

std::string bg_rgb(int r, int g, int b)
{
	return "\x1b[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
} // example: color::bg_rgb(100,100,100) gray background

std::string style(const std::string &modifier, const std::string &text_color, const std::string &background_color = "")
{
	return modifier + text_color + background_color;
} // example: std:: cout << color::style(color::modifier::BOLD,color::text::YELLOW,color::background::WHITE) << "Hello World!"<<color::modifier::RESET << std::endl;
} // namespace color

#endif // COLOR_HPP
