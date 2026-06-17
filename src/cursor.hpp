#pragma once

#include <iostream>
#include <string>

namespace cursor {
inline void up(int n = 1) { std::cout << "\033[" << n << "A"; }
inline void down(int n = 1) { std::cout << "\033[" << n << "B"; }
inline void right(int n = 1) { std::cout << "\033[" << n << "C"; }
inline void left(int n = 1) { std::cout << "\033[" << n << "D"; }

inline void home() { std::cout << "\033[H"; }
inline void moveTo(int row, int col) {
  std::cout << "\033[" << row << ";" << col << "H";
}

inline void clearLine() { std::cout << "\033[2K"; }
inline void clearToEnd() { std::cout << "\033[K"; }

inline void save() { std::cout << "\033[s"; }
inline void restore() { std::cout << "\033[u"; }

inline void hide() { std::cout << "\033[?25l"; }
inline void show() { std::cout << "\033[?25h"; }

// flush is separate so you can batch moves without flushing each one
inline void flush() { std::cout.flush(); }

/*
@ update line N from the bottom
@ lines are tracked bottom-up: line 1 = directly above cursor
@ line 0 = does nothing
*/
inline void updateLine(int linesFromBottom, const std::string &content) {
  std::cout << "\033[" << linesFromBottom << "A" // up
            << "\r\033[2K"                       // clear the line
            << content                           // write new content
            << "\033[" << linesFromBottom << "B" // back down
            << std::flush;
}

} // namespace cursor