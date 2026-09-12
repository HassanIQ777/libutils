/* Part of https://github.com/HassanIQ777/libutils
Made on:     2026-08-27
Last update: 2026-09-12 */

#pragma once

#include <cstdlib>
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
  Terminal(int w, int h) : w(w), h(h) { triggerResizing(); }
  Terminal() : w(0), h(0) { autoResize(); }

  void resize(int w) {
    this->w = w;
    triggerResizing();
  }

  void resize(int w, int h) {
    this->w = w;
    this->h = h;
    triggerResizing();
  }

  void autoResize() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (hStdOut == INVALID_HANDLE_VALUE ||
        !GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
      w = 80;
      h = 24;
    } else {
      w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
      h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#else
    struct winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_row == 0) {
      w = 80;
      h = 24;
    } else {
      w = static_cast<int>(ws.ws_col);
      h = static_cast<int>(ws.ws_row);
    }
#endif
    triggerResizing();
  }

  void draw() const {
    for (int row = 0; row < h; row++) {
      for (int col = 0; col < w; col++) {
        std::cout << getPixel(col, row);
      }
      std::cout << std::endl;
    }
  }

  void setPixel(int x, int y, const std::string &character) {
    if (x < 0 || y < 0 || x >= w || y >= h)
      return;
    terminal[y][x] = character;
  }

  std::string getPixel(int x, int y) const {
    if (x < 0 || y < 0 || x >= w || y >= h)
      return "";
    return terminal[y][x];
  }

  int getw() const { return w; }
  int geth() const { return h; }

  void clear() {
    terminal.clear();
    triggerResizing();
  }

  void clearScreen() {
#ifdef _WIN32
    system("cls")
#else
    system("clear");
#endif
  }

private:
  void triggerResizing() {
    terminal.resize(h);
    for (int i = 0; i < h; i++) {
      terminal[i].resize(w, " ");
    }
  }
};