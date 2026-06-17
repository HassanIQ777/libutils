#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/* you can use these as loading bars:
{"⠋", "⠙", "⠹", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
{"🕐","🕑","🕒","🕓","🕔","🕕","🕖","🕗","🕘","🕙","🕚","🕛"}
{"|", "/", "-", "\\"}
{".  ", ".. ", "..."}
{"▏","▎","▍","▌","▋","▊","▉"}
{"✶", "✸", "✹", "✺", "✹", "✸"}
{"(●    )", "( ●   )", "(  ●  )", "(   ● )", "(    ●)", "(   ● )", "(  ●  )",
"(●   )"}
{"ᗧ···", " ᗧ··", "  ᗧ·", "   ᗧ", "  ᗧ·", " ᗧ··"}
{"|", "!", "I", "1", "l", "|"}
{"[          ]", "[=         ]", "[==        ]", "[===       ]","[====
]","[=====     ]", "[======    ]", "[=======   ]", "[========  ]", "[=========
]","[==========]"}

@ these ones are different:
@ "▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂"
@ "← ↖ ↑ ↗ → ↘ ↓ ↙"
@ "▉ ▊ ▋ ▌ ▍ ▎ ▏ ▎ ▍ ▌ ▋ ▊"
@ "▖ ▘ ▝ ▗"
@ "┤ ┘ ┴ └ ├ ┌ ┬ ┐"
@ "◢ ◣ ◤ ◥"
@ "◰ ◳ ◲ ◱"
@ "⣾ ⣽ ⣻ ⢿ ⡿ ⣟ ⣯ ⣷"
@ "⠁ ⠂ ⠄ ⡀ ⢀ ⠠ ⠐ ⠈"
@ "◴ ◷ ◶ ◵"
@ "◐ ◓ ◑ ◒"
@ "◡◡ ⊙⊙ ◠◠"
@ "⣾ ⣽ ⣻ ⢿ ⡿ ⣟ ⣯ ⣷"
@ "⠁ ⠂ ⠄ ⡀ ⢀ ⠠ ⠐ ⠈"

*/

namespace Loadingbar {

class Spinner {
  std::atomic<bool> done;
  std::string msg;
  std::mutex set_msg_mutex;
  std::thread thread;

public:
  Spinner(const std::vector<std::string> &loading_bar, int sleep_duration,
          const std::string &msg_ = "Loading")
      : done(false), msg(msg_),
        thread([this, loading_bar, sleep_duration]() { // capture by value
          size_t i = 0;
          while (!done.load(std::memory_order_acquire)) {
            std::cout << "\033[2K" << "\r"
                      << loading_bar[i % loading_bar.size()] << " " << msg
                      << std::flush;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(sleep_duration));
            i++;
          }
          // std::cout << "\r✓ " << " done!          \n" << std::flush;
        }) {}

  void stop() {
    done = true;
    if (thread.joinable())
      thread.join();
  }

  void setMsg(const std::string &new_msg) {
    std::lock_guard<std::mutex> lock(set_msg_mutex);
    msg = new_msg;
  }

  ~Spinner() { stop(); }

  Spinner(const Spinner &) = delete;
  Spinner &operator=(const Spinner &) = delete;
}; // end of class Spinner

class StatusLine {
  std::atomic<bool> done;
  std::string msg;
  std::mutex set_msg_mutex;
  std::thread thread;

public:
  StatusLine(int sleep_duration, const std::string &msg_ = "Loading")
      : done(false), msg(msg_),
        thread([this, sleep_duration]() { // capture by value
          size_t i = 0;
          while (!done.load(std::memory_order_acquire)) {
            std::cout << "\033[2K"
                      << "\r" << msg << std::flush;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(sleep_duration));
            i++;
          }
          // std::cout << "\r✓ " << " done!          \n" << std::flush;
        }) {}

  void stop() {
    done = true;
    if (thread.joinable())
      thread.join();
  }

  void setMsg(const std::string &new_msg) {
    std::lock_guard<std::mutex> lock(set_msg_mutex);
    msg = new_msg;
  }

  ~StatusLine() { stop(); }

  StatusLine(const StatusLine &) = delete;
  StatusLine &operator=(const StatusLine &) = delete;

}; // end of class StatusLine

} // namespace Loadingbar