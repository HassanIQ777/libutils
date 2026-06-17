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
  std::mutex msg_mutex;
  std::thread thread;

public:
  Spinner(const std::vector<std::string> &loading_bar, int sleep_duration,
          const std::string &msg_ = "Loading")
      : done(false), msg(msg_),
        thread([this, loading_bar, sleep_duration]() { // capture by value
          size_t i = 0;
          while (!done.load(std::memory_order_acquire)) {
            std::string snapshot;
            {
              std::lock_guard<std::mutex> lock(msg_mutex);
              snapshot = msg;
            }
            std::cout << "\033[2K" << "\r"
                      << loading_bar[i % loading_bar.size()] << " " << snapshot
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
    std::lock_guard<std::mutex> lock(msg_mutex);
    msg = new_msg;
  }

  ~Spinner() { stop(); }

  Spinner(const Spinner &) = delete;
  Spinner &operator=(const Spinner &) = delete;
}; // end of class Spinner

class StatusLine {
  std::atomic<bool> done;
  std::string msg;
  std::mutex msg_mutex; // guards msg — both read AND write
  std::thread thread;

public:
  StatusLine(int sleep_ms, const std::string &initial_msg = "Loading")
      : done(false), msg(initial_msg), thread([this, sleep_ms]() {
          while (!done.load(std::memory_order_acquire)) {
            // snapshot under lock — release lock before I/O
            std::string snapshot;
            {
              std::lock_guard<std::mutex> lock(msg_mutex);
              snapshot = msg;
            }
            std::cout << "\033[2K\r" << snapshot << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
          }
          std::cout << "\033[2K\r"
                    << std::flush; // wipe line on exit — leave it clean
        }) {}

  void setMsg(const std::string &new_msg) {
    std::lock_guard<std::mutex> lock(msg_mutex);
    msg = new_msg;
  }

  void stop() {
    done = true;
    if (thread.joinable())
      thread.join();
  }

  ~StatusLine() { stop(); }

  StatusLine(const StatusLine &) = delete;
  StatusLine &operator=(const StatusLine &) = delete;
}; // end of class StatusLine

} // namespace Loadingbar