/* Part of https://github.com/HassanIQ777/libutils
Made on:     2026-08-27
Last update: 2026-08-28 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif
#include <unistd.h>

class Terminal {
private:
  int w, h;
  std::vector<std::vector<std::string>> terminal;

public:
  Terminal(int w, int h) : w(w), h(h) {}

  // resizes Terminal width
  void resize(int w) { this->w = w; }

  // resizes both Terminal width and height
  void resize(int w, int h) {
    this->w = w;
    this->h = h;
  }

  void autoResize() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
      this->w = 80;
      this->h = 24;
      return;
    }

    CONSOLE_Terminal_BUFFER_INFO csbi;
    if (!GetConsoleTerminalBufferInfo(hStdOut, &csbi)) {
      this->w = 80;
      this->h = 24;
      return;
    }

    return static_cast<size_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#else
    struct winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1 || w.ws_row == 0) {
      this->w = 80;
      this->h = 24;
      return;
    }

    this->w = static_cast<int>(w.ws_col);
    this->h = static_cast<int>(w.ws_row);
#endif
  }

  void draw() const {
    for (int row = 0; row < h; row++) {
      for (int col = 0; col < w; col++) {
        std::cout << terminal[row][col];
      }
      std::cout << std::endl;
    }
  }

  void setPixel(int x, int y, const std::string &character) {
    terminal[x][y] = character;
  }

  void clear() { terminal.clear(); }
};